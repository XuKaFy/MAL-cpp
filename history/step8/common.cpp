#include "common.h"

Type AbstractType::type() const
{
    return Type::TYPE_NULL;
}

AbstractType::~AbstractType()
{
    ;
}

AbstractType*  AbstractType::copy() const
{
    return new AbstractType();
}

Type FloatType::type() const
{
    return Type::TYPE_FLOAT;
}

AbstractType* FloatType::copy() const
{
    return new FloatType(m_num);
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

FloatType::~FloatType()
{
    ;
}

AtomType::AtomType(Atom n)
    : m_atom(n) {
}

Type AtomType::type() const
{
    return Type::TYPE_ATOM;
}

AbstractType* AtomType::copy() const
{
    return new AtomType(m_atom);
}

Atom AtomType::atom() const
{
    return m_atom;
}

void AtomType::setAtom(Atom n)
{
    m_atom = n;
}

AtomType::~AtomType()
{
    ;
}

ListType::ListType(List n)
    : m_list(n) {
}

Type ListType::type() const
{
    return Type::TYPE_LIST;
}

AbstractType* ListType::copy() const
{
    List ans = {nullptr, nullptr};
    if(m_list.first != nullptr)
        ans.first = m_list.first->copy();
    if(m_list.second != nullptr)
        ans.second = m_list.second->copy();
    return new ListType(ans);
}

List ListType::list() const
{
    return m_list;
}

void ListType::setList(List n)
{
    m_list = n;
}

ListType::~ListType()
{
    ;
}

StringType::StringType(String n)
    : m_str(n) {
}

Type StringType::type() const
{
    return Type::TYPE_STRING;
}

AbstractType* StringType::copy() const
{
    return new StringType(m_str);
}

String StringType::string() const
{
    return m_str;
}

void StringType::setString(String n)
{
    m_str = n;
}

StringType::~StringType()
{
    ;
}

LambdaType::LambdaType(ListType *arg, ListType* body)
    : m_arg(arg), m_body(body) {
}

Type LambdaType::type() const
{
    return Type::TYPE_LAMBDA;
}

AbstractType* LambdaType::copy() const
{
    return new LambdaType(static_cast<ListType*>(m_arg->copy()), 
                          static_cast<ListType*>(m_body->copy()));
}

ListType* LambdaType::arg() const
{
    return m_arg;
}

void LambdaType::setArg(ListType* arg)
{
    m_arg = arg;
}

ListType* LambdaType::body() const
{
    return m_body;
}

void LambdaType::setBody(ListType* body)
{
    m_body = body;
}

LambdaType::~LambdaType()
{
    ;
}

BuildinFunctionType::BuildinFunctionType(Function f, String name)
    : m_f(f), m_name(name) {
}

Type BuildinFunctionType::type() const
{
    return Type::TYPE_BUILDIN_FUNCTION;
}

AbstractType* BuildinFunctionType::copy() const
{
    return new BuildinFunctionType(m_f, m_name);
}

String BuildinFunctionType::name() const
{
    return m_name;
}

AbstractType* BuildinFunctionType::process(ListType* obj)
{
    return m_f(obj);
}

BuildinFunctionType::~BuildinFunctionType()
{
    ;
}

MacroType::MacroType(ListType *args, ListType *body)
    : LambdaType(args, body) {
}

Type MacroType::type() const
{
    return Type::TYPE_MACRO;
}

MacroType::~MacroType()
{
    ;
}