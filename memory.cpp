#include "memory.h"

Pointer<AbstractType> Memory::dispatchVoid()
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH AbstractType\n");
#endif
    return new AbstractType();
}

Pointer<SymbolType> Memory::dispatchSymbol(Symbol Symbol)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH SymbolType %s\n", Symbol.c_str());
#endif
    return new SymbolType(Symbol);
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

Pointer<IntegerType> Memory::dispatchInteger(Integer num)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH IntegerType %d\n", num);
#endif
    return new IntegerType(num);
}

Pointer<StringType> Memory::dispatchString(String str)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH StringType %s\n", str.c_str());
#endif
    return new StringType(str);
}

Pointer<VectorType> Memory::dispatchVector(Vector vec)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH VectorType length = %d\n", (int)vec.size());
#endif
    return new VectorType(vec);
}

Pointer<MapType> Memory::dispatchMap(Map map)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH MapType length = %d\n", (int)map.size());
#endif
    return new MapType(map);
}

Pointer<KeywordType> Memory::dispatchKeyword(Keyword key)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH KeywordType = %s\n", key.c_str());
#endif
    return new KeywordType(key);
}

EnvironmentPointer Memory::dispatchEnvironment(EnvironmentPointer env)
{
#ifdef DISPATCH_DEBUG
    printf("DISPATCH Environment parent = %p\n", env.get());
#endif
    return new EnvironmentType(env);
}