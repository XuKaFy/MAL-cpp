#include "memory.h"

Pointer<AbstractType> Memory::dispatchVoid()
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH AbstractType\n");
#endif
    return new AbstractType();
}

Pointer<AtomType> Memory::dispatchAtom(Atom atom)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH AtomType %s\n", atom.c_str());
#endif
    return new AtomType(atom);
}

Pointer<BuildinType> Memory::dispatchBuildin(Function fun, String name)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH BuildinType %s\n", name.c_str());
#endif
    return new BuildinType(fun, name);
}

ListPointer Memory::dispatchList(ValuePointer first, ValuePointer second)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH ListType %p, %p\n", first.get(), second.get());
#endif
    return new ListType(first, second);
}

Pointer<LambdaType> Memory::dispatchLambda(ListPointer  args, 
                                           ListPointer  body,
                                           EnvironmentPointer env)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH LambdaType \n\t%s \n\t%s\n", Printer::castList(args).c_str(),
                                                  Printer::castList(body).c_str());
#endif
    return new LambdaType(args, body, env);
}

Pointer<MacroType> Memory::dispatchMacro(ListPointer  args, 
                                         ListPointer  body,
                                         EnvironmentPointer env)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH MacroType \n\t%s \n\t%s\n",  Printer::castList(args).c_str(),
                                                  Printer::castList(body).c_str());
#endif
    return new MacroType(args, body, env);
}

Pointer<NumberType> Memory::dispatchNumber(Number num)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH NumberType %f\n", num);
#endif
    return new NumberType(num);
}

Pointer<StringType> Memory::dispatchString(String str)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH StringType %s\n", str.c_str());
#endif
    return new StringType(str);
}

EnvironmentPointer Memory::dispatchEnvironment(EnvironmentPointer env)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH Environment parent = %p\n", env.get());
#endif
    return new EnvironmentType(env);
}