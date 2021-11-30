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

FloatType::FloatType(Float n)
    : m_number(n) {
}

Type FloatType::type() const
{
    return Type::TYPE_FLOAT;
}

ValuePointer FloatType::copy() const
{
    return new FloatType(m_number);
}

FloatType::~FloatType()
{
    ;
}

SymbolType::SymbolType(const Symbol &n)
    : m_symbol(n) {
}

Type SymbolType::type() const
{
    return Type::TYPE_SYMBOL;
}

ValuePointer SymbolType::copy() const
{
    return new SymbolType(m_symbol);
}

SymbolType::~SymbolType()
{
    ;
}

VectorType::VectorType(const Vector &v)
    : m_vector(v) {
}

Type VectorType::type() const
{
    return Type::TYPE_VECTOR;
}

ValuePointer VectorType::copy() const
{
    Vector v;
    for(auto i : m_vector)
        v.push_back(i->copy());
    return new VectorType(v);
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

ListType::~ListType()
{
    ;
}

StringType::StringType(const String &n)
    : SymbolType(n) {
}

Type StringType::type() const
{
    return Type::TYPE_STRING;
}

ValuePointer StringType::copy() const
{
    return new StringType(symbol());
}

StringType::~StringType()
{
    ;
}

KeywordType::KeywordType(const String &n)
    : SymbolType(n) {
}

Type KeywordType::type() const
{
    return Type::TYPE_KEYWORD;
}

ValuePointer KeywordType::copy() const
{
    return new KeywordType(symbol());
}

KeywordType::~KeywordType()
{
    ;
}

LambdaType::LambdaType(Pointer<ListType> arg, Pointer<ListType> body, EnvironmentPointer env)
    : m_arg(arg), m_body(body), m_environment(env) {
}

Type LambdaType::type() const
{
    return Type::TYPE_LAMBDA;
}

ValuePointer LambdaType::copy() const
{
    return new LambdaType(m_arg->copy().convert<ListType>(),
                          m_body->copy().convert<ListType>(),
                          m_environment);
}

LambdaType::~LambdaType()
{
    ;
}

BuildinType::BuildinType(Function f, const String &name)
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

MapType::MapType(const Map& m)
    : m_map(m) {
}

Type MapType::type() const
{
    return Type::TYPE_HASHMAP;
}

MapType::~MapType()
{
    ;
}

ValuePointer MapType::copy() const
{
    Map m;
    for(auto i : m_map)
        m[i.first] = i.second->copy();
    return new MapType(m);
}

IntegerType::IntegerType(Integer integer)
    : m_integer(integer) {
}

Type IntegerType::type() const
{
    return Type::TYPE_INTEGER;
}

ValuePointer IntegerType::copy() const
{
    return new IntegerType(m_integer);
}

IntegerType::~IntegerType()
{
    ;
}