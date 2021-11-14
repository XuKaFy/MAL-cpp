#ifndef PRINTER_H
#define PRINTER_H

#include <sstream>
#include "common.h"

class Printer
{
public:
    String print(AbstractType *obj);
    String printWithEscape(StringType* str);

private:
    String escape();
    int hexNumber(String::value_type c);
    int octNumber(String::value_type c);

    String castNumber(Number n);
    String castAtom(Atom n);
    String castList(ListType *n);
    String castBuildinFunction(BuildinFunctionType *n);
    String castString(String n);
    String castLambda(LambdaType* n);
};

#endif // PRINTER_H