#include "memory.h"

AbstractType* Memory::dispatch()
{
    return new AbstractType();
}

AtomType* Memory::dispatch(Atom atom)
{
    return new AtomType(atom);
}

BuildinFunctionType* Memory::dispatch(Function fun, String name)
{
    return new BuildinFunctionType(fun, name);
}

ListType* Memory::dispatch(AbstractType* first, AbstractType* second)
{
    return new ListType(first, second);
}

LambdaType* Memory::dispatch(ListType* args, ListType* body, Environment* env)
{
    return new LambdaType(args, body, env);
}

MacroType* Memory::dispatch(ListType* args, ListType* body, Environment* env, bool)
{
    return new MacroType(args, body, env);
}

NumberType* Memory::dispatch(Number num)
{
    return new NumberType(num);
}

StringType* Memory::dispatch(String str, bool)
{
    return new StringType(str);
}

Environment* Memory::dispatch(Environment* env)
{
    return new Environment(env);
}