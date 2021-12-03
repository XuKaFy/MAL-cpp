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
    static Pointer<SymbolType>          dispatchSymbol      (Symbol num);
    static Pointer<BuildinType>         dispatchBuildin     (Function fun, String name);
    static ListPointer                  dispatchList        (ValuePointer first    = nullptr, 
                                                             ValuePointer second   = nullptr);
    static Pointer<LambdaType>          dispatchLambda      (ListPointer  args, 
                                                             ListPointer  body,
                                                             EnvironmentPointer env);
    static Pointer<MacroType>           dispatchMacro       (ListPointer  args,
                                                             ListPointer  body,
                                                             EnvironmentPointer env);
    static Pointer<FloatType>           dispatchFloat       (Float num);
    static Pointer<IntegerType>         dispatchInteger     (Integer num);
    static Pointer<StringType>          dispatchString      (String str);
    static Pointer<VectorType>          dispatchVector      (Vector vec);
    static Pointer<MapType>             dispatchMap         (Map map);
    static Pointer<KeywordType>         dispatchKeyword     (Keyword key);
    static Pointer<AtomType>            dispatchAtom        (ValuePointer val);

    static EnvironmentPointer           dispatchEnvironment (EnvironmentPointer env);
};

#endif // MEMORY_H