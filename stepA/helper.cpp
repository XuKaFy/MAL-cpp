#include "helper.h"

ValuePointer Helper::car(ListPointer o)
{
    return o->first();
}

ValuePointer Helper::cdr(ListPointer o)
{
    return o->second();
}

bool Helper::Symbol(ValuePointer o)
{
    if(o->type() == Type::TYPE_SYMBOL)
        return true;
    if(isEmpty(o))
        return true;
    return false;
}

bool Helper::eq(ValuePointer a1, ValuePointer a2)
{
    if(!a1 && !a2)
        return true;
    if(!a1 || !a2)
        return false;
    if(a1->type() != a2->type())
        return false;
    switch(a1->type()) {
    case Type::TYPE_SYMBOL:
        return GETSYMBOL(a1) == GETSYMBOL(a2);
    case Type::TYPE_BUILDIN:
        return GETBUILDIN(a1)->name() == GETBUILDIN(a2)->name();
    case Type::TYPE_LIST:
        if(!eq(car(GETLIST(a1)), car(GETLIST(a2))))
            return false;
        if(!eq(cdr(GETLIST(a1)), cdr(GETLIST(a2))))
            return false;
        return true;
    case Type::TYPE_NULL:
        return true;
    case Type::TYPE_NUMBER:
        return GETNUMBER(a1) == GETNUMBER(a2);
    case Type::TYPE_STRING:
        return GETSTRING(a1) == GETSTRING(a2);
    case Type::TYPE_LAMBDA:
        return GETLAMBDA(a2)->body() == GETLAMBDA(a2)->body()
                && GETLAMBDA(a1)->arg() == GETLAMBDA(a2)->arg();  
    case Type::TYPE_MACRO:
        return GETMACRO(a2)->body() == GETMACRO(a2)->body()
                && GETMACRO(a1)->arg() == GETMACRO(a2)->arg();
    }
    return false;
}

ListPointer Helper::cons(ValuePointer a1, ValuePointer a2)
{
    return Memory::dispatchList(a1, a2);
}

ValuePointer Helper::get(ListPointer& o)
{
    if(isEmpty(o))
        throw Exception("Helper::get: Can't get 1 elem");
    ValuePointer f = car(o);
    next(o);
    return f;
}

void Helper::next(ListPointer &o)
{
    o = GETLIST(cdr(o));
}

bool Helper::isEmpty(ValuePointer o)
{
    if(o->type() != Type::TYPE_LIST)
        return false;
    return isEmpty(GETLIST(o));
}

bool Helper::isEmpty(ListPointer o)
{
    return !car(o) && !cdr(o);
}

bool Helper::isLast(ListPointer o)
{
    if(!cdr(o))
        return true;
    if(cdr(o)->type() != Type::TYPE_LIST)
        return true;
    return isEmpty(cdr(o));
}

bool Helper::isList(ListPointer o)
{
    if(!isLast(o))
        return isList(GETLIST(cdr(o)));
    return isSingle(o);
}

bool Helper::isSingle(ListPointer o)
{
    if(isEmpty(o))
        return false;
    return isEmpty(cdr(o));
}

bool Helper::isSelfEvaluating(ValuePointer o)
{
    if(o->type() == Type::TYPE_NUMBER)
        return true;
    if(o->type() == Type::TYPE_STRING)
        return true;
    if(o->type() == Type::TYPE_BUILDIN)
        return true;
    if(o->type() == Type::TYPE_LAMBDA)
        return true;
    if(o->type() == Type::TYPE_MACRO)
        return true;
    return false;
}

bool Helper::isTrue(ValuePointer o)
{
    return !isFalse(o);
}

bool Helper::isFalse(ValuePointer o)
{
    return isEmpty(o);
}

bool Helper::isFlat(ListPointer o)
{
    if(isEmpty(o))
        return true;
    if(car(o)->type() != Type::TYPE_SYMBOL)
        return false;
    if(!isLast(o))
        return isFlat(GETLIST(cdr(o)));
    return isSingle(o);
}

ValuePointer Helper::constantVoid()
{
    static Pointer<AbstractType> val(Memory::dispatchVoid());
    return val;
}

ValuePointer Helper::constantTrue()
{
    static Pointer<SymbolType> val(Memory::dispatchSymbol("t"));
    return VALUE(val);
}

ValuePointer Helper::constantFalse()
{
    static ListPointer val(Memory::dispatchList());
    return VALUE(val);
}

ValuePointer Helper::foreach(ListPointer o, std::function<void(ValuePointer o)> f)
{
    if(isEmpty(o))
        return VALUE(o);
    while(!isLast(o)) {
        f(car(o));
        next(o);
    }
    f(car(o));
    return cdr(o);
}

ListPointer Helper::append(ListPointer o, ValuePointer n)
{
    if(isEmpty(o)) {
        o->setFirst(n);
        return o;
    }
    ListPointer back = Memory::dispatchList(n);
    o->setSecond(VALUE(back));
    return back;
}