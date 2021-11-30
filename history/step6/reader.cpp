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
    Float k = 0;
    bool read = false;
    while(remain() && isdigit(lookahead())) {
        k = k * 10 + lookahead() - '0';
        match(lookahead());
        read = true;
    }
    if(!read)
        throw Exception("Analyzer::number: No Float");
    return new FloatType(k);
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
    throw Exception("Analyzer::atom: No Atom");
}

AbstractType* Analyzer::list()
{
    match('(');
    ListType* root = new ListType();
    ListType* current = root;
    while(remain() && lookahead() != ')') {
        current = Helper::append(current, elem());
        delSpace();
    }
    if(Helper::car(current) != nullptr)
        current->setList(List{Helper::car(current), new ListType()});
    match(')');
    return root;
}

AbstractType* Analyzer::string()
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
    return new StringType(s);
}

AbstractType* Analyzer::elem()
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

AbstractType* Reader::read(String s)
{
    Analyzer az;
    return az.analyze(s);
}