#ifndef HELPER_H
#define HELPER_H

#include "common.h"
#include "memory.h"

#define GETNUMBER(o) (Helper::convert<NumberType*>(o, Type::TYPE_NUMBER)->number())
#define GETSTRING(o) (Helper::convert<StringType*>(o, Type::TYPE_STRING)->string())
#define GETATOM(o) (Helper::convert<AtomType*>(o, Type::TYPE_ATOM)->atom())
#define GETBUILDIN(o) (Helper::convert<BuildinFunctionType*>(o, Type::TYPE_BUILDIN_FUNCTION))
#define GETLAMBDA(o) (Helper::convert<LambdaType*>(o, Type::TYPE_LAMBDA))
#define GETLIST(o) (Helper::convert<ListType*>(o, Type::TYPE_LIST))
#define GETMACRO(o) (Helper::convert<MacroType*>(o, Type::TYPE_MACRO))

#define CAR(o) (Helper::car(o))
#define CDR(o) (Helper::cdr(o))
#define CONS(a1, a2) (Helper::cons(a1, a2))
#define EQ(a1, a2) (Helper::eq(a1, a2))
#define ISEMPTY(o) (Helper::isEmpty(o))
#define ISLIST(o) (Helper::isList(o))
#define ISTRUE(o) (Helper::isTrue(o))
#define ISFALSE(o) (Helper::isFalse(o))

#define GET(o) (Helper::get(o))
#define SINGLE(ans, o) \
    if(Helper::isEmpty(o) || !Helper::isSingle(o)) \
        throw Exception("SINGLE: Not single arg"); \
    AbstractType* ans = CAR(o);
#define DOUBLE(a1, a2, o) \
    AbstractType* a1 = GET(o); \
    AbstractType* a2 = GET(o); \
    if(!Helper::isEmpty(o)) \
        throw Exception("DOUBLE: Not double args");
#define NONEARG(o) \
    if(!Helper::isEmpty(o)) \
        throw Exception("NONEARG: Shouldn't give arg");
#define FUNCTION(o) [](ListType* o) -> AbstractType*
#define FOREACH(o, m, block) (Helper::foreach(m, [&](AbstractType* o) block))

#define TRUE (Helper::constantTrue())
#define FALSE (Helper::constantFalse())
#define IF(x) ((x) ? TRUE : FALSE)

#define QUOTE(x) (Memory::dispatch(Memory::dispatch("quote"), Memory::dispatch(x, Memory::dispatch(nullptr, nullptr))))
#define UNQUOTE(x) (Memory::dispatch(Memory::dispatch("unquote"), Memory::dispatch(x, Memory::dispatch(nullptr, nullptr))))
#define SPLICE_UNQUOTE(x) (Memory::dispatch(Memory::dispatch("splice-unquote"), Memory::dispatch(x, Memory::dispatch(nullptr, nullptr))))
#define QUASIQUOTE(x) (Memory::dispatch(Memory::dispatch("quasiquote"), Memory::dispatch(x, Memory::dispatch(nullptr, nullptr))))

#define BEGIN(x) (Memory::dispatch(Memory::dispatch("begin"), x))

class Helper
{
public:
    static AbstractType* car(ListType* o);
    static AbstractType* cdr(ListType* o);
    static bool atom(AbstractType* o);
    static bool eq(AbstractType* a1, AbstractType* a2);
    static ListType* cons(AbstractType* a1, AbstractType* a2);

    static AbstractType* get(ListType *& o);
    static void next(ListType *& o);

    static bool isEmpty(AbstractType* o);
    static bool isLast(ListType* o);
    static bool isList(ListType* o);
    static bool isSingle(ListType *o);
    static bool isSelfEvaluating(AbstractType* o);
    static bool isTrue(AbstractType* o);
    static bool isFalse(AbstractType* o);
    static bool isFlat(ListType* o);

    static AbstractType* constantVoid();
    static AbstractType* constantTrue();
    static AbstractType* constantFalse();
    
    static ListType* append(ListType* o, AbstractType* n);
    static AbstractType* foreach(ListType* o, std::function<void(AbstractType* o)> f);

    template<typename T>
    static T convert(AbstractType* o, Type t) {
        if(o == nullptr)
            throw Exception("convert: Can't convert \"nullptr\"");
        if(o->type() != t)
            throw Exception("convert: Wrong type");
        return static_cast<T>(o);
    }

    template<typename T>
    static T convert(AbstractType* o) {
        if(o == nullptr)
            throw Exception("convert: Can't convert \"nullptr\"");
        return static_cast<T>(o);
    }
};

#endif //HELPER_H