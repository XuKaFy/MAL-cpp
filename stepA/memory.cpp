#include "memory.h"

Pointer<AbstractType> Memory::dispatch()
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH AbstractType\n");
#endif
    return new AbstractType();
}

Pointer<AtomType> Memory::dispatch(Atom atom)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH AtomType %s\n", atom.c_str());
#endif
    return new AtomType(atom);
}

Pointer<BuildinType> Memory::dispatch(Function fun, String name)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH BuildinType %s\n", name.c_str());
#endif
    return new BuildinType(fun, name);
}

Pointer<ListType> Memory::dispatch(ValueType first, ValueType second)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH ListType %p, %p\n", first, second);
#endif
    return new ListType(first, second);
}

Pointer<LambdaType> Memory::dispatch(Pointer<ListType> args, 
                                     Pointer<ListType> body, 
                                     Pointer<Environment> env)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH LambdaType \n\t%s \n\t%s\n", Printer::print(args).c_str(),
                                                  Printer::print(body).c_str());
#endif
    return new LambdaType(args, body, env);
}

Pointer<MacroType> Memory::dispatch(Pointer<ListType> args, 
                                    Pointer<ListType> body, 
                                    Pointer<Environment> env, bool)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH MacroType \n\t%s \n\t%s\n",  Printer::print(args).c_str(),
                                                  Printer::print(body).c_str());
#endif
    return new MacroType(args, body, env);
}

Pointer<NumberType> Memory::dispatch(Number num)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH NumberType %f\n", num);
#endif
    return new NumberType(num);
}

Pointer<StringType> Memory::dispatch(String str, bool)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH StringType %s\n", str.c_str());
#endif
    return new StringType(str);
}

Pointer<Environment> Memory::dispatch(Pointer<Environment> env)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH Environment parent = %p\n", env);
#endif
    return new Environment(env);
}