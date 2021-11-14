#ifndef PRINTER_H
#define PRINTER_H

#include <sstream>
#include "common.h"

class Printer
{
public:
    static String print(AbstractType *obj);
    static String printWithEscape(StringType* str);

private:
    static String escape();
    static int hexNumber(String::value_type c);
    static int octNumber(String::value_type c);

    static String castNumber(Number n);
    static String castAtom(Atom n);
    static String castList(ListType *n);
    static String castBuildinFunction(BuildinFunctionType *n);
    static String castString(String n);
    static String castLambda(LambdaType* n);
};

#endif // PRINTER_H