#include "type.h"

Type AbstractType::type() const
{
    return Type::TYPE_NULL;
}

AbstractType::~AbstractType()
{
    ;
}

ValuePointer AbstractType::copy() const
{
    return new AbstractType();
}

Type NumberType::type() const
{
    return Type::TYPE_NUMBER;
}

ValuePointer NumberType::copy() const
{
    return new NumberType(m_num);
}

NumberType::NumberType(Number n)
    : m_num(n) {
}

Number NumberType::number() const
{
    return m_num;
}

void NumberType::setNumber(Number n)
{
    m_num = n;
}

NumberType::~NumberType()
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

ValuePointer AtomType::copy() const
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

ListType::ListType(ValuePointer first, ValuePointer second)
    : m_first(first), m_second(second) {
}

Type ListType::type() const
{
    return Type::TYPE_LIST;
}

ValuePointer ListType::copy() const
{
    ValuePointer first, second;
    if(m_first)
        first = m_first->copy();
    if(m_second)
        second = m_second->copy();
    return new ListType(first, second);
}

ValuePointer ListType::first() const
{
    return m_first;
}

void ListType::setFirst(ValuePointer first)
{
    m_first = first;
}

ValuePointer ListType::second() const
{
    return m_second;
}

void ListType::setSecond(ValuePointer second)
{
    m_second = second;
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

ValuePointer StringType::copy() const
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

LambdaType::LambdaType(Pointer<ListType> arg, Pointer<ListType> body, EnvironmentPointer env)
    : m_arg(arg), m_body(body), m_env(env) {
}

Type LambdaType::type() const
{
    return Type::TYPE_LAMBDA;
}

ValuePointer LambdaType::copy() const
{
    return new LambdaType(m_arg->copy().convert<ListType>(),
                          m_body->copy().convert<ListType>(),
                          m_env);
}

Pointer<ListType> LambdaType::arg() const
{
    return m_arg;
}

void LambdaType::setArg(Pointer<ListType> arg)
{
    m_arg = arg;
}

Pointer<ListType> LambdaType::body() const
{
    return m_body;
}

void LambdaType::setBody(Pointer<ListType> body)
{
    m_body = body;
}

EnvironmentPointer LambdaType::environment() const
{
    return m_env;
}

void LambdaType::setEnvironment(EnvironmentPointer env)
{
    m_env = env;
}

LambdaType::~LambdaType()
{
    ;
}

BuildinType::BuildinType(Function f, String name)
    : m_f(f), m_name(name) {
}

Type BuildinType::type() const
{
    return Type::TYPE_BUILDIN;
}

ValuePointer BuildinType::copy() const
{
    return new BuildinType(m_f, m_name);
}

String BuildinType::name() const
{
    return m_name;
}

ValuePointer BuildinType::process(Pointer<ListType> obj)
{
    return m_f(obj);
}

BuildinType::~BuildinType()
{
    ;
}

MacroType::MacroType(Pointer<ListType> arg, Pointer<ListType> body, EnvironmentPointer env)
    : LambdaType(arg, body, env) {
}

Type MacroType::type() const
{
    return Type::TYPE_MACRO;
}

MacroType::~MacroType()
{
    ;
}