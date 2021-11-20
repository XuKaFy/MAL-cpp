#include "helper.h"

AbstractType* Helper::car(ListType* o)
{
    return o->first();
}

AbstractType* Helper::cdr(ListType* o)
{
    return o->second();
}

bool Helper::atom(AbstractType* o)
{
    if(o->type() == Type::TYPE_ATOM)
        return true;
    if(isEmpty(o))
        return true;
    return false;
}

bool Helper::eq(AbstractType* a1, AbstractType* a2)
{
    if(a1 == nullptr && a2 == nullptr)
        return true;
    if(a1 == nullptr || a2 == nullptr)
        return false;
    if(a1->type() != a2->type())
        return false;
    switch(a1->type()) {
    case Type::TYPE_ATOM:
        return GETATOM(a1) == GETATOM(a2);
    case Type::TYPE_BUILDIN_FUNCTION:
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
    return Helper::constantVoid();
}

ListType* Helper::cons(AbstractType* a1, AbstractType* a2)
{
    return Memory::dispatch(a1, a2);
}

AbstractType* Helper::get(ListType*& o)
{
    if(isEmpty(o))
        throw Exception("Helper::get: Can't get 1 elem");
    AbstractType* f = car(o);
    next(o);
    return f;
}

void Helper::next(ListType *& o)
{
    o = GETLIST(cdr(o));
}

bool Helper::isEmpty(AbstractType *o)
{
    if(o->type() != Type::TYPE_LIST)
        return false;
    ListType* l = GETLIST(o);
    return car(l) == nullptr && cdr(l) == nullptr;
}

bool Helper::isLast(ListType *o)
{
    if(cdr(o) == nullptr)
        return true;
    if(cdr(o)->type() != Type::TYPE_LIST)
        return true;
    return isEmpty(cdr(o));
}

bool Helper::isList(ListType* o)
{
    if(!isLast(o))
        return isList(GETLIST(cdr(o)));
    return isSingle(o);
}

bool Helper::isSingle(ListType* o)
{
    if(isEmpty(o))
        return false;
    return isEmpty(cdr(o));
}

bool Helper::isSelfEvaluating(AbstractType* o)
{
    if(o->type() == Type::TYPE_NUMBER)
        return true;
    if(o->type() == Type::TYPE_STRING)
        return true;
    if(o->type() == Type::TYPE_BUILDIN_FUNCTION)
        return true;
    if(o->type() == Type::TYPE_LAMBDA)
        return true;
    if(o->type() == Type::TYPE_MACRO)
        return true;
    return false;
}

bool Helper::isTrue(AbstractType* o)
{
    return !isFalse(o);
}

bool Helper::isFalse(AbstractType* o)
{
    return isEmpty(o);
}

bool Helper::isFlat(ListType* o)
{
    if(isEmpty(o))
        return true;
    if(car(o)->type() != Type::TYPE_ATOM)
        return false;
    if(!isLast(o))
        return isFlat(GETLIST(cdr(o)));
    return isSingle(o);
}

AbstractType* Helper::constantVoid()
{
    static std::shared_ptr<AbstractType> val(Memory::dispatch());
    return val.get();
}

AbstractType* Helper::constantTrue()
{
    static std::shared_ptr<AbstractType> val(Memory::dispatch("t"));
    return val.get();
}

AbstractType* Helper::constantFalse()
{
    static std::shared_ptr<AbstractType> val(Memory::dispatch(nullptr, nullptr));
    return val.get();
}

AbstractType* Helper::foreach(ListType* o, std::function<void(AbstractType* o)> f)
{
    if(isEmpty(o))
        return o;
    while(!isLast(o)) {
        f(car(o));
        next(o);
    }
    f(car(o));
    return cdr(o);
}

ListType* Helper::append(ListType* o, AbstractType* n)
{
    if(isEmpty(o)) {
        o->setFirst(n);
        return o;
    }
    ListType* back = Memory::dispatch(n, nullptr);
    o->setSecond(back);
    return back;
}