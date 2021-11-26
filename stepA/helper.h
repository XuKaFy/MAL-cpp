#ifndef HELPER_H
#define HELPER_H

#include "common.h"
#include "pointer.h"
#include "memory.h"

#define CONVERT(o, type)    (o.convert<type>()                        )
#define VALUE(o)            (CONVERT(o,    AbstractType)              )

#define GETNUMBER(o)        (CONVERT(o,    NumberType)     ->number() )
#define GETSTRING(o)        (CONVERT(o,    StringType)     ->string() )
#define GETATOM(o)          (CONVERT(o,    AtomType)       ->atom()   )
#define GETBUILDIN(o)       (CONVERT(o,    BuildinType)               )
#define GETLAMBDA(o)        (CONVERT(o,    LambdaType)                )
#define GETLIST(o)          (CONVERT(o,    ListType)                  )
#define GETMACRO(o)         (CONVERT(o,    MacroType)                 )

#define CAR(o)              (Helper::car(o)             )
#define CDR(o)              (Helper::cdr(o)             )
#define CONS(a1, a2)        (Helper::cons(a1, a2)       )
#define EQ(a1, a2)          (Helper::eq(a1, a2)         )
#define ISEMPTY(o)          (Helper::isEmpty(o)         )
#define ISLIST(o)           (Helper::isList(o)          )
#define ISTRUE(o)           (Helper::isTrue(o)          )
#define ISFALSE(o)          (Helper::isFalse(o)         )

#define TRUE                (Helper::constantTrue()     )
#define FALSE               (Helper::constantFalse()    )
#define VOID                (Helper::constantVoid()     )
#define IF(x)               ((x) ? TRUE : FALSE)

#define MAKELIST(a, b)      (Memory::dispatch(VALUE(a), VALUE(b)))

#define QUOTE(x)            MAKELIST(Memory::dispatch("quote"),             MAKELIST(x, FALSE))
#define UNQUOTE(x)          MAKELIST(Memory::dispatch("unquote"),           MAKELIST(x, FALSE))
#define SPLICE_UNQUOTE(x)   MAKELIST(Memory::dispatch("splice-unquote"),    MAKELIST(x, FALSE))
#define QUASIQUOTE(x)       MAKELIST(Memory::dispatch("quasiquote"),        MAKELIST(x, FALSE))

#define BEGIN(x)            MAKELIST(Memory::dispatch("begin"), x)

#define GET(o) (Helper::get(o))
#define SINGLE(ans, o) \
    if(ISEMPTY(o) || !Helper::isSingle(o)) \
        throw Exception("SINGLE: Not single arg"); \
    ValueType ans = CAR(o);
#define DOUBLE(a1, a2, o) \
    ValueType a1 = GET(o); \
    ValueType a2 = GET(o); \
    if(!ISEMPTY(o)) \
        throw Exception("DOUBLE: Not double args");
#define NONEARG(o) \
    if(!ISEMPTY(o)) \
        throw Exception("NONEARG: Shouldn't give arg");
#define FUNCTION(o) [](Pointer<ListType> o) -> ValueType
#define FOREACH(o, m, block) (Helper::foreach(m, [&](ValueType o) -> void block))

class Helper
{
public:
    static ValueType car(Pointer<ListType> o);
    static ValueType cdr(Pointer<ListType> o);
    static bool atom(ValueType o);
    static bool eq(ValueType a1, ValueType a2);
    static Pointer<ListType> cons(ValueType a1, ValueType a2);

    static ValueType get(Pointer<ListType> &o);
    static void next(Pointer<ListType> &o);

    static bool isEmpty(ValueType o);
    static bool isEmpty(Pointer<ListType> o);
    static bool isLast(Pointer<ListType> o);
    static bool isList(Pointer<ListType> o);
    static bool isSingle(Pointer<ListType> o);
    static bool isSelfEvaluating(ValueType o);
    static bool isTrue(ValueType o);
    static bool isFalse(ValueType o);
    static bool isFlat(Pointer<ListType> o);

    static ValueType constantVoid();
    static ValueType constantTrue();
    static ValueType constantFalse();
    
    static Pointer<ListType> append(Pointer<ListType> o, ValueType n);
    static ValueType foreach(Pointer<ListType> o, std::function<void(ValueType o)> f);
};

#endif //HELPER_H