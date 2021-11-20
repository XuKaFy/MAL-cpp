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
class BuildinFunctionType;

class Environment;

class Memory
{
public:
    static AbstractType*        dispatch();
    static AtomType*            dispatch(Atom num);
    static BuildinFunctionType* dispatch(Function fun, String name);
    static ListType*            dispatch(AbstractType* first, AbstractType* second);
    static LambdaType*          dispatch(ListType* args, ListType* body,  Environment* env);
    static MacroType*           dispatch(ListType* args, ListType* body,  Environment* env, bool);
    static NumberType*          dispatch(Number num);
    static StringType*          dispatch(String str, bool);

    static Environment*         dispatch(Environment* env);
};

#endif // MEMORY_H