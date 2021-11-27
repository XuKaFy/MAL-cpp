#ifndef PRINTER_H
#define PRINTER_H

#include <sstream>

#include "type.h"
#include "helper.h"

class Printer
{
public:
    static String print(ValuePointer obj);
    static String printWithEscape(String str);

private:
    static String escape();
    static int    hexNumber(String::value_type c);
    static int    octNumber(String::value_type c);

    static String castNumber(Number n);
    static String castAtom(Atom n);
    static String castList(Pointer<ListType> n);
    static String castBuildinFunction(Pointer<BuildinType> n);
    static String castString(String n);
    static String castLambda(Pointer<LambdaType> n);
    static String castMacro(Pointer<MacroType> n);
};

#endif // PRINTER_H