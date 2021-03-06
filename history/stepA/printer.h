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

    static String castFloat(Float n);
    static String castSymbol(Symbol n);
    static String castList(ListPointer n);
    static String castBuildinFunction(Pointer<BuildinType> n);
    static String castString(String n);
    static String castLambda(Pointer<LambdaType> n);
    static String castMacro(Pointer<MacroType> n);

private:
    static String escape();
    static int    hexFloat(String::value_type c);
    static int    octFloat(String::value_type c);

};

#endif // PRINTER_H