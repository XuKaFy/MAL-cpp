#ifndef PRINTER_H
#define PRINTER_H

#include <sstream>
#include "common.h"

class Printer
{
public:
    String print_str(AbstractType *obj);

private:
    String castFloat(Float n);
    String castAtom(Atom n);
    String castList(AbstractType *n); // Assume that n is type "ListType"
};

#endif // PRINTER_H