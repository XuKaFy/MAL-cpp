#include "reader.h"

AbstractType* Analyzer::analyze(String s)
{
    m_s = s;
    m_pos = 0;
    m_len = s.size();

    return elem();
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

AbstractType* Analyzer::number()
{
    Number k = 0;
    bool read = false;
    while(remain() && isdigit(lookahead())) {
        k = k * 10 + lookahead() - '0';
        match(lookahead());
        read = true;
    }
    if(!read)
        throw Exception::EXP_ANA_NONE;
    return new NumberType(k);
}

AbstractType* Analyzer::atom()
{
    String s;
    if(remain() && isAtomHead(lookahead())) {
        s += lookahead();
        match(lookahead());
        while(remain() && isAtomBody(lookahead())) {
            s += lookahead();
            match(lookahead());
        }
        return new AtomType(s);
    }
    throw Exception::EXP_ANA_NONE;
}

AbstractType* Analyzer::list()
{
    match("(");
    ListType* root = new ListType();
    ListType* current = root;
    while(remain() && lookahead() != ')') {
        AbstractType* obj = elem();
        ListType* l = new ListType();
        current->setList(List(obj, l));
        current = l;
        delSpace();
        // The last of the list is '()
    }
    match(")");
    return root;
}

AbstractType* Analyzer::elem()
{
    delSpace();
    if(!remain())
        throw Exception::EXP_READ_MATCH_MORE;
    if(lookahead() == '(') {
        return list();
    } else if(isdigit(lookahead())) {
        return number();
    } else if(isAtomHead(lookahead())) {
        return atom();
    } else {
        throw Exception::EXP_ANA_MATCHLESS;
    }
}

bool Analyzer::isSymbol(String::value_type c)
{
    if(c == '+') return true;
    if(c == '-') return true;
    if(c == '*') return true;
    if(c == '/') return true;
    if(c == '%') return true;
    if(c == '^') return true;
    return false;
}

bool Analyzer::isAtomHead(String::value_type c)
{
    return isSymbol(c) || isalpha(c) || c == '_';
}

bool Analyzer::isAtomBody(String::value_type c)
{
    return isalnum(c) || c == '_';
}

String::value_type Analyzer::lookahead() const
{
    if(m_pos == m_len) {
        throw Exception::EXP_READ_MATCH_MORE;
    }
    return m_s[m_pos];
}

void Analyzer::match(String::value_type k)
{
    if(k == lookahead()) {
        ++m_pos;
    } else {
        throw Exception::EXP_READ_MATCH_ERROR;
    }
}

void Analyzer::match(String k)
{
    for(auto i : k)
        match(i);
}

AbstractType* Reader::read_str(String s)
{
    Analyzer az;
    return az.analyze(s);
}