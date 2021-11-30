#ifndef HELPER_H
#define HELPER_H

#include "type.h"
#include "pointer.h"
#include "memory.h"

#define CONVERT(o, type)    (o.convert<type>())

#define VALUE(o)            CONVERT(o,    AbstractType)
#define GETFLOAT(o)         CONVERT(o,    FloatType)      ->number()
#define GETINTEGER(o)       CONVERT(o,    IntegerType)    ->integer()
#define GETSTRING(o)        CONVERT(o,    StringType)     ->symbol()
#define GETSYMBOL(o)        CONVERT(o,    SymbolType)     ->symbol()
#define GETBUILDIN(o)       CONVERT(o,    BuildinType)
#define GETLAMBDA(o)        CONVERT(o,    LambdaType)
#define GETLIST(o)          CONVERT(o,    ListType)
#define GETMACRO(o)         CONVERT(o,    MacroType)
#define GETVECTOR(o)        CONVERT(o,    VectorType)     ->vector()
#define GETKEYWORD(o)       CONVERT(o,    KeywordType)    ->symbol()
#define GETMAP(o)           CONVERT(o,    MapType)        ->map()

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

#define BEGIN(x)            MAKE_LIST(Memory::dispatchSymbol(SYM_BEGIN),        x)
#define QUOTE(x)            MAKE_LIST(Memory::dispatchSymbol(SYM_QUOTE),        MAKE_LIST(x, FALSE))
#define UNQUOTE(x)          MAKE_LIST(Memory::dispatchSymbol(SYM_UQ),           MAKE_LIST(x, FALSE))
#define SPLICE_UNQUOTE(x)   MAKE_LIST(Memory::dispatchSymbol(SYM_SUQ),          MAKE_LIST(x, FALSE))
#define QUASIQUOTE(x)       MAKE_LIST(Memory::dispatchSymbol(SYM_QQ),           MAKE_LIST(x, FALSE))

#define SINGLE(ans, o) \
    if(ISEMPTY(o) || !Helper::isSingle(o)) \
        throw Exception("SINGLE: Not single arg"); \
    ValuePointer ans = CAR(o);
#define DOUBLE(a1, a2, o) \
    ValuePointer a1 = GET(o); \
    ValuePointer a2 = GET(o); \
    if(!ISEMPTY(o)) \
        throw Exception("DOUBLE: Not double args");
#define DOUBLE_FROM_VALUE(a1, a2, o) \
    ListPointer o##Temp = GETLIST(o); \
    ValuePointer a1 = GET(o##Temp); \
    ValuePointer a2 = GET(o##Temp); \
    if(!ISEMPTY(o##Temp)) \
        throw Exception("DOUBLE_FROM_VALUE: Not double args");
#define TRIBLE_FROM_VALUE(a1, a2, a3, o) \
    ListPointer o##Temp = GETLIST(o); \
    ValuePointer a1 = GET(o##Temp); \
    ValuePointer a2 = GET(o##Temp); \
    ValuePointer a3 = GET(o##Temp); \
    if(!ISEMPTY(o##Temp)) \
        throw Exception("TRIBLE_FROM_VALUE: Not trible args");
#define NONEARG(o) \
    if(!ISEMPTY(o)) \
        throw Exception("NONEARG: Shouldn't give arg");
#define FUNCTION(o) [](ListPointer o) -> ValuePointer
#define FOREACH(o, m, block) (Helper::foreach(m, [&](ValuePointer o) -> void block))

class Helper
{
public:
    static ValuePointer          car        (ListPointer o);
    static ValuePointer          cdr        (ListPointer o);
    static bool                  symbol     (ValuePointer o);
    static bool                  eq         (ValuePointer a1,   ValuePointer a2);
    static bool                  eqHashmap  (const Map &a1,     const Map& a2);
    static bool                  eqVector   (const Vector& a1,  const Vector& a2);
    static ListPointer           cons       (ValuePointer a1,   ValuePointer a2);

    static ValuePointer          get    (ListPointer &o);
    static void                  next   (ListPointer &o);

    static bool isEmpty          (ValuePointer o);
    static bool isEmpty          (ListPointer o);
    static bool isLast           (ListPointer o);
    static bool isList           (ListPointer o);
    static bool isSingle         (ListPointer o);
    static bool isSelfEvaluating (ValuePointer o);
    static bool isTrue           (ValuePointer o);
    static bool isFalse          (ValuePointer o);
    static bool isFlat           (ListPointer o);

    static int           count   (ListPointer o);
    static ValuePointer  nth     (ListPointer o, int cnt);

    static ValuePointer constantVoid();
    static ValuePointer constantTrue();
    static ValuePointer constantFalse();
    
    static ListPointer  append   (ListPointer o, ValuePointer n);
    static ValuePointer foreach  (ListPointer o, std::function<void(ValuePointer o)> f);
};

#endif //HELPER_H