#include "reader.h"

ValuePointer Analyzer::analyze(String s)
{
    m_s = s;
    m_pos = 0;
    m_len = s.size();

    ValuePointer ans = nullptr;
    while(ans.empty() && remain())
        ans = elem();
    if(remain()) {
        ListPointer root = Memory::dispatchList();
        ListPointer current = root;
        current = Helper::append(current, ans);
        while(remain()) {
            ans = elem();
            if(!ans.empty())
                current = Helper::append(current, ans);
        }
        current->setSecond(FALSE);
        return VALUE(BEGIN(root));
    }
    if(ans.empty())
        return VOID;
    return ans;
}

bool Analyzer::remain() const
{
    return m_pos < m_len;
}

void Analyzer::delSpace()
{
    while(remain() && isspace(lookahead()))
        match(lookahead());
}

ValuePointer Analyzer::number()
{
    Integer k = 0;
    bool read = false;
    while(remain() && isdigit(lookahead())) {
        k = k * 10 + lookahead() - '0';
        match(lookahead());
        read = true;
    }
    if(!read)
        throw Exception("Analyzer::number: No Float");
    if(remain() && lookahead() == '.') {
        match(lookahead());
        Float m = GETINTEGER(number());
        while(m > 1) m /= 10;
        return VALUE(Memory::dispatchFloat(k + m));
    }
    return VALUE(Memory::dispatchInteger(k));
}

ValuePointer Analyzer::symbol()
{
    String s;
    if(remain() && isSymbolHead(lookahead())) {
        s += lookahead();
        match(lookahead());
        while(remain() && isSymbolBody(lookahead())) {
            s += lookahead();
            match(lookahead());
        }
        return VALUE(Memory::dispatchSymbol(s));
    }
    throw Exception("Analyzer::Symbol: No Symbol");
}

ValuePointer Analyzer::list()
{
    match('(');
    ListPointer root = Memory::dispatchList();
    ListPointer current = root;
    while(remain() && lookahead() != ')') {
        ValuePointer a = elem();
        if(!a.empty())
            current = Helper::append(current, a);
        delSpace();
    }
    if(CAR(current))
        current->setSecond(FALSE);
    match(')');
    return VALUE(root);
}

ValuePointer Analyzer::vector()
{
    match('[');
    Vector root;
    while(remain() && lookahead() != ']') {
        ValuePointer a = elem();
        if(!a.empty())
            root.push_back(a);
        delSpace();
    }
    match(']');
    return VALUE(Memory::dispatchVector(root));
}

ValuePointer Analyzer::string()
{
    String s;
    match('"');
    while(remain() && lookahead() != '"') {
        if(lookahead() == '\\') {
            s += lookahead();
            match(lookahead());
            if(lookahead() == '"') { // \" must be processed in reader
                s += lookahead();
                match(lookahead());
            }
        } else {
            s += lookahead();
            match(lookahead());
        }
    }
    match('"');
    return VALUE(Memory::dispatchString(s));
}

ValuePointer Analyzer::keyword()
{
    match(':');
    return VALUE(Memory::dispatchKeyword(GETSYMBOL(symbol())));
}

ValuePointer Analyzer::map()
{
    match('{');
    Map root;
    while(remain() && lookahead() != '}') {
        ValuePointer k = keyword();
        delSpace();
        ValuePointer v = elem();
        if(v.empty())
            throw Exception("Analyzer::map: How can elem be null?");
        delSpace();
        root[GETKEYWORD(k)] = v;
    }
    match('}');
    return VALUE(Memory::dispatchMap(root));
}

ValuePointer Analyzer::elem()
{
    delSpace();
    if(!remain())
        throw Exception("Analyzer::elem: No elem");
    if(lookahead() == '(') {
        return list();
    } else if(isdigit(lookahead())) {
        return number();
    } else if(isSymbolHead(lookahead())) {
        return symbol();
    } else if(lookahead() == '"') {
        return string();
    } else if(lookahead() == '\'') {
        match(lookahead());
        return VALUE(QUOTE(elem()));
    } if(lookahead() == SYM_SIM_QQ[0]) {
        match(lookahead());
        return VALUE(QUASIQUOTE(elem()));
    } if(lookahead() == SYM_SIM_SUQ[0]) {
        match(lookahead());
        if(lookahead() == SYM_SIM_SUQ[1]) {
            match(lookahead());
            return VALUE(SPLICE_UNQUOTE(elem()));
        }
        return VALUE(UNQUOTE(elem()));
    } else if(lookahead() == SYM_COMMENT[0]) {
        match(lookahead());
        comment();
        return nullptr;
    } else if(lookahead() == '[') {
        return VALUE(vector());
    } else if(lookahead() == '{') {
        return VALUE(map());
    } else if(lookahead() == ':') {
        return VALUE(keyword());
    } else {
        throw Exception("Analyzer::elem: Can't match an elem");
    }
}

void Analyzer::comment()
{
    while(remain() && lookahead() != '\n')
        match(lookahead());
}

bool Analyzer::isSymbol(String::value_type c)
{
    if(c == '_') return true;
    if(c == '+') return true;
    if(c == '-') return true;
    if(c == '*') return true;
    if(c == '/') return true;
    if(c == '%') return true;
    if(c == '^') return true;
    if(c == '<') return true;
    if(c == '>') return true;
    if(c == '=') return true;
    if(c == '?') return true;
    if(c == '!') return true;
    if(c == '&') return true;
    if(c == '.') return true;
    if(c == '|') return true;
    return false;
}

bool Analyzer::isSymbolHead(String::value_type c)
{
    return isSymbol(c) || isalpha(c);
}

bool Analyzer::isSymbolBody(String::value_type c)
{
    return isalnum(c) || isSymbol(c);
}

String::value_type Analyzer::lookahead() const
{
    if(m_pos == m_len) {
        throw Exception("Analyzer::lookahead: At the end");
    }
    return m_s[m_pos];
}

void Analyzer::match(String::value_type k)
{
    if(k == lookahead()) {
        ++m_pos;
    } else {
        throw Exception("Analyzer::match: Wrong match");
    }
}

void Analyzer::match(String k)
{
    for(auto i : k)
        match(i);
}

ValuePointer Reader::read(String s)
{
    Analyzer az;
    return az.analyze(s);
}