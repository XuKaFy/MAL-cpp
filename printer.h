#ifndef PRINTER_H
#define PRINTER_H

#include <sstream>

#include "type.h"
#include "helper.h"

class Printer
{
public:
    static String print(ValuePointer obj, bool readably = false);
    static String printWithEscape(const String &str);

    static String castFloat             (Float n,                   bool readably);
    static String castInteger           (Integer n,                 bool readably);
    static String castSymbol            (const Symbol &n,           bool readably);
    static String castList              (ListPointer n,             bool readably);
    static String castBuildinFunction   (Pointer<BuildinType> n,    bool readably);
    static String castString            (const String &n,           bool readably);
    static String castLambda            (Pointer<LambdaType> n,     bool readably);
    static String castMacro             (Pointer<MacroType> n,      bool readably);
    static String castVector            (const Vector &n,           bool readably);
    static String castMap               (const Map &n,              bool readably);
    static String castKeyword           (const Keyword &n,          bool readably);
    static String castAtom              (ValuePointer ref,          bool readably);

private:
    static String escape();
    static int    hexFloat(String::value_type c);
    static int    octFloat(String::value_type c);

};

#endif // PRINTER_H