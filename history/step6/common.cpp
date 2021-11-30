#include "common.h"

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

LambdaType::LambdaType(ListType *arg, ListType* body)
    : m_arg(arg), m_body(body) {
}

Type LambdaType::type() const
{
    return Type::TYPE_LAMBDA;
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