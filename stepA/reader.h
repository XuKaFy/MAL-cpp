#ifndef READER_H
#define READER_H

#include "common.h"
#include "helper.h"

class Analyzer
{
public:
    ValueType analyze(String s);

private:
    void match(String k);
    void match(String::value_type k);
    String::value_type lookahead() const;

    ValueType atom();
    ValueType list();
    ValueType number();
    ValueType string();
    ValueType elem();

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
    static ValueType read(String s);
};

#endif // READER_H