#ifndef READER_H
#define READER_H

#include "type.h"
#include "helper.h"

class Analyzer
{
public:
    ValuePointer analyze(String s);

private:
    void match(String k);
    void match(String::value_type k);
    String::value_type lookahead() const;

    ValuePointer atom();
    ValuePointer list();
    ValuePointer number();
    ValuePointer string();
    ValuePointer elem();

    bool remain() const;
    void delSpace();

    static bool isSymbol(String::value_type c);
    static bool isAtomHead(String::value_type c);
    static bool isAtomBody(String::value_type c);

    String m_s;
    size_t m_pos;
    size_t m_len;
};

class Reader
{
public:
    static ValuePointer read(String s);
};

#endif // READER_H