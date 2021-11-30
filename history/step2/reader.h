#ifndef READER_H
#define READER_H

#include "common.h"

/*
    atom    ->  [_A-Za-z]
            |   atom [_0-9A-Za-z]
            |   epsilon
    number  ->  [0-9]
            |   number [0-9]
            |   epsilon
    list    ->  list
            |   list elem
            |   epsilon
    str     ->  "ALL_KIND_OF_CHARACTORS"
    elem    ->  number
            |   atom
            |   (list)
            |   epsilon
*/

class Analyzer
{
public:
    AbstractType* analyze(String s);

private:
    void match(String k);
    void match(String::value_type k);
    String::value_type lookahead() const;

    AbstractType* atom();
    AbstractType* list();
    AbstractType* number();
    AbstractType* string();
    AbstractType* elem();

    String escape();
    int hexFloat();
    int octFloat();

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
    AbstractType* read_str(String s);
};

#endif // READER_H