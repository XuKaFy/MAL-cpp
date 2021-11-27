#include "reader.h"

ValuePointer Analyzer::analyze(String s)
{
    m_s = s;
    m_pos = 0;
    m_len = s.size();

    ValuePointer ans = elem();
    if(remain()) {
        ListPointer root = Memory::dispatchList();
        ListPointer current = root;
        current = Helper::append(current, ans);
        while(remain()) {
            ans = elem();
            current = Helper::append(current, ans);
        }
        current->setSecond(FALSE);
        return VALUE(BEGIN(root));
    }
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
    Number k = 0;
    bool read = false;
    while(remain() && isdigit(lookahead())) {
        k = k * 10 + lookahead() - '0';
        match(lookahead());
        read = true;
    }
    if(!read)
        throw Exception("Analyzer::number: No Number");
    if(remain() && lookahead() == '.') {
        match(lookahead());
        Number m = GETNUMBER(number());
        while(m > 1) m /= 10;
        return VALUE(Memory::dispatchNumber(k + m));
    }
    return VALUE(Memory::dispatchNumber(k));
}

ValuePointer Analyzer::atom()
{
    String s;
    if(remain() && isAtomHead(lookahead())) {
        s += lookahead();
        match(lookahead());
        while(remain() && isAtomBody(lookahead())) {
            s += lookahead();
            match(lookahead());
        }
        return VALUE(Memory::dispatchAtom(s));
    }
    throw Exception("Analyzer::atom: No Atom");
}

ValuePointer Analyzer::list()
{
    match('(');
    ListPointer root = Memory::dispatchList();
    ListPointer current = root;
    while(remain() && lookahead() != ')') {
        current = Helper::append(current, elem());
        delSpace();
    }
    if(CAR(current))
        current->setSecond(FALSE);
    match(')');
    return VALUE(root);
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

ValuePointer Analyzer::elem()
{
    delSpace();
    if(!remain())
        throw Exception("Analyzer::elem: No elem");
    if(lookahead() == '(') {
        return list();
    } else if(isdigit(lookahead())) {
        return number();
    } else if(isAtomHead(lookahead())) {
        return atom();
    } else if(lookahead() == '"') {
        return string();
    } else if(lookahead() == '\'') {
        match(lookahead());
        return VALUE(QUOTE(elem()));
    } if(lookahead() == '`') {
        match(lookahead());
        return VALUE(QUASIQUOTE(elem()));
    } if(lookahead() == '~') {
        match(lookahead());
        if(lookahead() == '@') {
            match(lookahead());
            return VALUE(SPLICE_UNQUOTE(elem()));
        }
        return VALUE(UNQUOTE(elem()));
    } else {
        throw Exception("Analyzer::elem: Can't match an elem");
    }
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
    return false;
}

bool Analyzer::isAtomHead(String::value_type c)
{
    return isSymbol(c) || isalpha(c);
}

bool Analyzer::isAtomBody(String::value_type c)
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