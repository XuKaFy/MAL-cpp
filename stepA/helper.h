#ifndef HELPER_H
#define HELPER_H

#include "type.h"
#include "pointer.h"
#include "memory.h"

#define CONVERT(o, type)    (o.convert<type>())

#define VALUE(o)            (CONVERT(o,    AbstractType)              )
#define GETNUMBER(o)        (CONVERT(o,    NumberType)     ->number() )
#define GETSTRING(o)        (CONVERT(o,    StringType)     ->string() )
#define GETATOM(o)          (CONVERT(o,    AtomType)       ->atom()   )
#define GETBUILDIN(o)       (CONVERT(o,    BuildinType)               )
#define GETLAMBDA(o)        (CONVERT(o,    LambdaType)                )
#define GETLIST(o)          (CONVERT(o,    ListType)                  )
#define GETMACRO(o)         (CONVERT(o,    MacroType)                 )

#define CAR(o)              Helper::car(o)
#define CDR(o)              Helper::cdr(o)
#define CONS(a1, a2)        Helper::cons(a1, a2)
#define EQ(a1, a2)          Helper::eq(a1, a2)
#define ISEMPTY(o)          Helper::isEmpty(o)
#define ISLIST(o)           Helper::isList(o)
#define ISTRUE(o)           Helper::isTrue(o)
#define ISFALSE(o)          Helper::isFalse(o)
#define GET(o)              Helper::get(o)
#define NEXT(o)             Helper::next(o)

#define TRUE                Helper::constantTrue()
#define FALSE               Helper::constantFalse()
#define VOID                Helper::constantVoid()
#define IF(x)               ((x) ? TRUE : FALSE)

#define MAKE_LIST(a, b)     (Memory::dispatchList(VALUE(a), VALUE(b)))

#define BEGIN(x)            MAKE_LIST(Memory::dispatchAtom("begin"),             x)
#define QUOTE(x)            MAKE_LIST(Memory::dispatchAtom("quote"),             MAKE_LIST(x, FALSE))
#define UNQUOTE(x)          MAKE_LIST(Memory::dispatchAtom("unquote"),           MAKE_LIST(x, FALSE))
#define SPLICE_UNQUOTE(x)   MAKE_LIST(Memory::dispatchAtom("splice-unquote"),    MAKE_LIST(x, FALSE))
#define QUASIQUOTE(x)       MAKE_LIST(Memory::dispatchAtom("quasiquote"),        MAKE_LIST(x, FALSE))

#define SINGLE(ans, o) \
    if(ISEMPTY(o) || !Helper::isSingle(o)) \
        throw Exception("SINGLE: Not single arg"); \
    ValuePointer ans = CAR(o);
#define DOUBLE(a1, a2, o) \
    ValuePointer a1 = GET(o); \
    ValuePointer a2 = GET(o); \
    if(!ISEMPTY(o)) \
        throw Exception("DOUBLE: Not double args");
#define NONEARG(o) \
    if(!ISEMPTY(o)) \
        throw Exception("NONEARG: Shouldn't give arg");
#define FUNCTION(o) [](ListPointer o) -> ValuePointer
#define FOREACH(o, m, block) (Helper::foreach(m, [&](ValuePointer o) -> void block))

class Helper
{
public:
    static ValuePointer             car    (ListPointer o);
    static ValuePointer             cdr    (ListPointer o);
    static bool                     atom   (ValuePointer o);
    static bool                     eq     (ValuePointer a1, ValuePointer a2);
    static ListPointer              cons   (ValuePointer a1, ValuePointer a2);

    static ValuePointer             get    (ListPointer &o);
    static void                     next   (ListPointer &o);

    static bool isEmpty          (ValuePointer o);
    static bool isEmpty          (ListPointer o);
    static bool isLast           (ListPointer o);
    static bool isList           (ListPointer o);
    static bool isSingle         (ListPointer o);
    static bool isSelfEvaluating (ValuePointer o);
    static bool isTrue           (ValuePointer o);
    static bool isFalse          (ValuePointer o);
    static bool isFlat           (ListPointer o);

    static ValuePointer constantVoid();
    static ValuePointer constantTrue();
    static ValuePointer constantFalse();
    
    static ListPointer  append   (ListPointer o, ValuePointer n);
    static ValuePointer foreach  (ListPointer o, std::function<void(ValuePointer o)> f);
};

#endif //HELPER_H