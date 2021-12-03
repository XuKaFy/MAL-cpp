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

    static String castFloat             (Float n,                   bool readably = true);
    static String castInteger           (Integer n,                 bool readably = true);
    static String castSymbol            (const Symbol &n,           bool readably = true);
    static String castList              (ListPointer n,             bool readably = true);
    static String castBuildinFunction   (Pointer<BuildinType> n,    bool readably = true);
    static String castString            (const String &n,           bool readably = true);
    static String castLambda            (Pointer<LambdaType> n,     bool readably = true);
    static String castMacro             (Pointer<MacroType> n,      bool readably = true);
    static String castVector            (const Vector &n,           bool readably = true);
    static String castMap               (const Map &n,              bool readably = true);
    static String castKeyword           (const Keyword &n,          bool readably = true);
    static String castAtom              (ValuePointer ref,          bool readably = true);

private:
    static String escape();
    static int    hexFloat(String::value_type c);
    static int    octFloat(String::value_type c);

};

#endif // PRINTER_H