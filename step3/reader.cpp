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
        current = Helper::append(current, elem());
        delSpace();
    }
    if(Helper::car(current) != nullptr)
        current->setList(List{Helper::car(current), new ListType()});
    match(")");
    return root;
}

AbstractType* Analyzer::string()
{
    String s;
    match('"');
    while(remain() && lookahead() != '"') {
        if(lookahead() == '\\') {
            s += escape();
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
        throw Exception::EXP_READ_MATCH_MORE;
    if(lookahead() == '(') {
        return list();
    } else if(isdigit(lookahead())) {
        return number();
    } else if(isAtomHead(lookahead())) {
        return atom();
    } else if(lookahead() == '"') {
        return string();
    } else {
        throw Exception::EXP_ANA_MATCHLESS;
    }
}

String Analyzer::escape()
{
    String ans;
    match('\\');
    switch(lookahead()) {
    case 'a':
        ans += '\a';
        match(lookahead());
        break;
    case 'b':
        ans += '\b';
        match(lookahead());
        break;
    case 'f':
        ans += '\f';
        match(lookahead());
        break;
    case 'n':
        ans += '\n';
        match(lookahead());
        break;
    case 'r':
        ans += '\r';
        match(lookahead());
        break;
    case 't':
        ans += '\t';
        match(lookahead());
        break;
    case 'v':
        ans += '\v';
        match(lookahead());
        break;
    case '\\':
        ans += '\\';
        match(lookahead());
        break;
    case '?':
        ans += '\?';
        match(lookahead());
        break;
    case '\'':
        ans += '\'';
        match(lookahead());
        break;
    case '"':
        ans += '"';
        match(lookahead());
        break;
    case '0':
        match(lookahead());
        if(isdigit(lookahead())) {
            int con = 0;
            for(int i=0; i<2; ++i)
                con = con * 8 + octNumber();
            ans += con;
        } else {
            ans += '\0';
        }
        break;
    default:
        if(!isalnum(lookahead()))
            throw Exception::EXP_ANA_ESCAPE_ERROR;
        int con = 0;
        if(lookahead() == 'x') {
            match('x');
            for(int i=0; i<2; ++i)
                con = con * 16 + hexNumber();
            ans += String::value_type(con / 16);
            ans += String::value_type(con % 16);
        } else {
            for(int i=0; i<3; ++i)
                con = con * 8 + octNumber();
            ans += con;
        }
        break;
    }
    return ans;
}

int Analyzer::hexNumber()
{
    int ans = 0;
    if(isdigit(lookahead())) {
        ans = lookahead() - '0';
    } else if(isalpha(lookahead())) {
        ans = tolower(lookahead()) - 'a';
        if(ans >= 16)
            throw Exception::EXP_ANA_ESCAPE_ERROR;
    }
    match(lookahead());
    return ans;
}

int Analyzer::octNumber()
{
    if(!isdigit(lookahead()))
        throw Exception::EXP_ANA_ESCAPE_ERROR;
    int ans = lookahead() - '0';
    match(lookahead());
    return ans;
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
    return isalnum(c) || c == '_' || c == '!' || c == '*' || c == '?';
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