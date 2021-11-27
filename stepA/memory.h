#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>

#include "type.h"
#include "environment.h"

#ifdef DISPATCH_DEBUG
#include "printer.h"
#endif

class Memory
{
public:
    static Pointer<AbstractType>        dispatchVoid        ();
    static Pointer<AtomType>            dispatchAtom        (Atom num);
    static Pointer<BuildinType>         dispatchBuildin     (Function fun, String name);
    static ListPointer                  dispatchList        (ValuePointer first    = nullptr, 
                                                             ValuePointer second   = nullptr);
    static Pointer<LambdaType>          dispatchLambda      (ListPointer  args, 
                                                             ListPointer  body,
                                                             EnvironmentPointer env);
    static Pointer<MacroType>           dispatchMacro       (ListPointer  args,
                                                             ListPointer  body,
                                                             EnvironmentPointer env);
    static Pointer<NumberType>          dispatchNumber      (Number num);
    static Pointer<StringType>          dispatchString      (String str);

    static EnvironmentPointer           dispatchEnvironment (EnvironmentPointer env);
};

#endif // MEMORY_H