#ifndef READER_H
#define READER_H

#include "type.h"
#include "helper.h"

class Analyzer
{
public:
    ValuePointer analyze(String::iterator &begin, String::iterator end);

private:
    void match(String k);
    void match(String::value_type k);
    String::value_type lookahead() const;

    ValuePointer symbol();
    ValuePointer list();
    ValuePointer number();
    ValuePointer string();
    ValuePointer vector();
    ValuePointer keyword();
    ValuePointer map();
    ValuePointer elem();
    void         comment();

    bool remain() const;
    void delSpace();

    static bool isSymbol(String::value_type c);
    static bool isSymbolHead(String::value_type c);
    static bool isSymbolBody(String::value_type c);

    String::iterator m_begin;
    String::iterator m_end;
};

class Reader
{
public:
    static ValuePointer read(String s);
};

#endif // READER_H