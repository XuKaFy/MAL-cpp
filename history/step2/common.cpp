#include "common.h"

Environment::Environment(Environment* parent)
    : m_parent(parent) {
    ;
}

Environment* Environment::parent() const
{
    return m_parent;
}

void Environment::setParent(Environment* parent)
{
    m_parent = parent;
}

AbstractType* Environment::getValue(String s)
{
    if(!m_map.count(s)) {
        if(parent() != nullptr) {
            return parent()->getValue(s);
        } else {
            throw Exception::EXP_ENV_NOT_FOUND;
        }
    }
    return m_map[s];
}

void Environment::setValue(String s, AbstractType* val)
{
    m_map[s] = val;
}

Type AbstractType::type() const
{
    return Type::TYPE_NULL;
}

Type FloatType::type() const
{
    return Type::TYPE_FLOAT;
}

FloatType::FloatType(Float n)
    : m_num(n) {
}

Float FloatType::number() const
{
    return m_num;
}

void FloatType::setFloat(Float n)
{
    m_num = n;
}

AtomType::AtomType(Atom n)
    : m_atom(n) {
}

Type AtomType::type() const
{
    return Type::TYPE_ATOM;
}

Atom AtomType::atom() const
{
    return m_atom;
}

void AtomType::setAtom(Atom n)
{
    m_atom = n;
}

ListType::ListType(List n)
    : m_list(n) {
}

Type ListType::type() const
{
    return Type::TYPE_LIST;
}

List ListType::list() const
{
    return m_list;
}

void ListType::setList(List n)
{
    m_list = n;
}

StringType::StringType(String n)
    : m_str(n) {
}

Type StringType::type() const
{
    return Type::TYPE_STRING;
}

String StringType::string() const
{
    return m_str;
}

void StringType::setString(String n)
{
    m_str = n;
}

BuildinFunctionType::BuildinFunctionType(Function f, String name)
    : m_f(f), m_name(name) {
}

Type BuildinFunctionType::type() const
{
    return Type::TYPE_BUILDIN_FUNCTION;
}

String BuildinFunctionType::name() const
{
    return m_name;
}

AbstractType* BuildinFunctionType::process(ListType* obj)
{
    return m_f(obj);
}

AbstractType* Helper::car(ListType* o)
{
    return o->list().first;
}

AbstractType* Helper::cdr(ListType* o)
{
    return o->list().second;
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
        return convert<AtomType*>(a1)->atom() == convert<AtomType*>(a2)->atom();
    case Type::TYPE_BUILDIN_FUNCTION:
        return convert<BuildinFunctionType*>(a1)->name() 
            == convert<BuildinFunctionType*>(a2)->name();
    case Type::TYPE_LIST:
        if(!eq(car(convert<ListType*>(a1)), car(convert<ListType*>(a2))))
            return false;
        if(!eq(cdr(convert<ListType*>(a1)), cdr(convert<ListType*>(a2))))
            return false;
        return true;
    case Type::TYPE_NULL:
        return true;
    case Type::TYPE_FLOAT:
        return convert<FloatType*>(a1)->number() == convert<FloatType*>(a2)->number();
    case Type::TYPE_STRING:
        return convert<StringType*>(a1)->string() == convert<StringType*>(a2)->string();    
    }
    return new AbstractType();
}

ListType* Helper::cons(AbstractType* a1, AbstractType* a2)
{
    return new ListType(List{a1, a2});
}

AbstractType* Helper::get(ListType*& o)
{
    if(isEmpty(o))
        throw Exception::EXP_HELPER_GET_ERROR;
    AbstractType* f = car(o);
    next(o);
    return f;
}

void Helper::next(ListType *& o)
{
    o = convert<ListType*>(cdr(o), Type::TYPE_LIST);
}

bool Helper::isEmpty(AbstractType *o)
{
    if(o->type() != Type::TYPE_LIST)
        return false;
    ListType* l = convert<ListType*>(o);
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
        return isList(convert<ListType*>(cdr(o), Type::TYPE_LIST));
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
    if(o->type() == Type::TYPE_FLOAT)
        return true;
    if(o->type() == Type::TYPE_STRING)
        return true;
    if(o->type() == Type::TYPE_BUILDIN_FUNCTION)
        return true;
    return false;
}

AbstractType* Helper::foreach(ListType* o, std::function<void(AbstractType* o)> f)
{
    if(isEmpty(o))
        return o;
    f(car(o));
    while(!isLast(o)) {
        next(o);
        f(car(o));
    }
    return cdr(o);
}

ListType* Helper::append(ListType* o, AbstractType* n)
{
    if(isEmpty(o)) {
        o->setList(List{n, nullptr});
        return o;
    }
    ListType* back = new ListType(List{n, nullptr});
    o->setList(List{car(o), back});
    return back;
}