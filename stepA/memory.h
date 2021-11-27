#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>

#include "type.h"
#include "environment.h"

class Memory
{
public:
    static Pointer<AbstractType>        dispatchVoid        ();
    static Pointer<AtomType>            dispatchAtom        (Atom num);
    static Pointer<BuildinType>         dispatchBuildin     (Function fun, String name);
    static Pointer<ListType>            dispatchList        (ValuePointer first    = nullptr, 
                                                             ValuePointer second   = nullptr);
    static Pointer<LambdaType>          dispatchLambda      (Pointer<ListType>  args, 
                                                             Pointer<ListType>  body,
                                                             EnvironmentPointer env);
    static Pointer<MacroType>           dispatchMacro       (Pointer<ListType>  args,
                                                             Pointer<ListType>  body,
                                                             EnvironmentPointer env);
    static Pointer<NumberType>          dispatchNumber      (Number num);
    static Pointer<StringType>          dispatchString      (String str);

    static EnvironmentPointer           dispatchEnvironment (EnvironmentPointer env);
};

#endif // MEMORY_H