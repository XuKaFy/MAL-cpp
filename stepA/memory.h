#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include "common.h"
#include "environment.h"

class AbstractType;
class NumberType;
class AtomType;
class ListType;
class LambdaType;
class MacroType;
class BuildinType;

class Environment;

class Memory
{
public:
    static Pointer<AbstractType>        dispatch();
    static Pointer<AtomType>            dispatch(Atom num);
    static Pointer<BuildinType>         dispatch(Function fun, String name);
    static Pointer<ListType>            dispatch(ValueType first, ValueType second);
    static Pointer<LambdaType>          dispatch(Pointer<ListType> args, 
                                                 Pointer<ListType> body,  
                                                 Pointer<Environment> env);
    static Pointer<MacroType>           dispatch(Pointer<ListType> args, 
                                                 Pointer<ListType> body,  
                                                 Pointer<Environment> env, bool);
    static Pointer<NumberType>          dispatch(Number num);
    static Pointer<StringType>          dispatch(String str, bool);

    static Pointer<Environment>         dispatch(Pointer<Environment> env);
};

#endif // MEMORY_H