#include "reader.h"

ValuePointer Analyzer::analyze(String::iterator &begin, String::iterator end)
{
    m_begin = begin;
    m_end = end;

    ValuePointer ans = elem();
    begin = m_begin;
    return ans;
}

bool Analyzer::remain() const
{
    return m_begin != m_end;
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
        return nullptr;
    if(lookahead() == '(') {
        return list();
    } else if(isdigit(lookahead())) {
        return number();
    } else if(lookahead() == '"') {
        return string();
    } else if(lookahead() == SYM_SIM_QT[0]) {
        match(lookahead());
        return VALUE(QUOTE(elem()));
    } else if(lookahead() == SYM_SIM_DEREF[0]) {
        match(lookahead());
        return VALUE(DEREF(elem()));
    } else if(lookahead() == SYM_SIM_QQ[0]) {
        match(lookahead());
        return VALUE(QUASIQUOTE(elem()));
    } else if(lookahead() == SYM_SIM_WM[0]) {
        match(lookahead());
        ValuePointer v = map();
        ValuePointer e = elem();
        return VALUE(WITH_META(v, e));
    } else if(lookahead() == SYM_SIM_SUQ[0]) {
        match(lookahead());
        if(lookahead() == SYM_SIM_SUQ[1]) {
            match(lookahead());
            return VALUE(SPLICE_UNQUOTE(elem()));
        }
        return VALUE(UNQUOTE(elem()));
    } else if(lookahead() == SYM_COMMENT[0]) {
        comment();
        return nullptr;
    } else if(isSymbolHead(lookahead())) {
        return symbol();
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
    match(';');
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
    if(!remain())
        throw Exception("Analyzer::lookahead: At the end");
    return *m_begin;
}

void Analyzer::match(String::value_type k)
{
    if(k == lookahead()) {
        ++m_begin;
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
    String::iterator b = s.begin();
    ListPointer root = Memory::dispatchList();
    ListPointer current = root;
    while(b != s.end()) {
        ValuePointer v = az.analyze(b, s.end());
        if(!v.empty())
            current = Helper::append(current, v);
    }
    if(ISEMPTY(root))
        return VOID;
    if(Helper::isLast(root))
        return CAR(root);
    if(CAR(current))
        current->setSecond(FALSE);
    return VALUE(BEGIN(root));
}