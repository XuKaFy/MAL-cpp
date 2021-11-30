#ifndef TYPE_H
#define TYPE_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

#include "definition.h"
#include "environment.h"
#include "pointer.h"

#include "debug.h"

class       AbstractType;
class       ListType;

typedef     Pointer<AbstractType>                           ValuePointer;
typedef     Pointer<ListType>                               ListPointer;
typedef     std::vector<ValuePointer>                       Vector;
typedef     std::unordered_map<Keyword, ValuePointer>       Map;
typedef     Environment<ValuePointer>                       EnvironmentType;
typedef     Pointer<EnvironmentType>                        EnvironmentPointer;
typedef     std::function<ValuePointer(Pointer<ListType>)>  Function;

struct StackFrame {
    ValuePointer        o;
    EnvironmentPointer  env;
};

#define ATTRIBUTE(type, nameB, name) \
public: \
    void set##nameB(type name) { \
        m_##name = name; \
    } \
    const type& name() const { \
        return m_##name; \
    } \
    type& name() { \
        return m_##name; \
    } \
private: \
    type m_##name;

#define ATTRIBUTE_READONLY(type, name) \
public: \
    const type& name() const { \
        return m_##name; \
    } \
private: \
    type m_##name;

class AbstractType
{
public:
    virtual Type            type() const;
    virtual ValuePointer    copy() const;

    virtual ~AbstractType();

private:
    Type m_type;
};

class FloatType : public AbstractType
{
public:
    FloatType(Float n = Float());

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    virtual ~FloatType();

ATTRIBUTE(Float, Float, number);
};

class IntegerType : public AbstractType
{
public:
    IntegerType(Integer n = Integer());

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    virtual ~IntegerType();

ATTRIBUTE(Integer, Integer, integer);
};

class SymbolType : public AbstractType
{
public:
    SymbolType(const Symbol &n = Symbol());

    virtual Type            type() const;
    virtual ValuePointer    copy() const;

    virtual ~SymbolType();

ATTRIBUTE(Symbol, Symbol, symbol);
};

class VectorType : public AbstractType
{
public:
    VectorType(const Vector &v = Vector());

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

ATTRIBUTE(Vector, Vector, vector)
};

class ListType : public AbstractType
{
public:
    ListType(ValuePointer first = nullptr, ValuePointer second = nullptr);

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    virtual ~ListType();

ATTRIBUTE(ValuePointer, First, first);
ATTRIBUTE(ValuePointer, Second, second);
};

class StringType : public SymbolType
{
public:
    StringType(const String &n = String());

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    virtual ~StringType();
};

class KeywordType : public SymbolType
{
public:
    KeywordType(const String &n = String());

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    virtual ~KeywordType();
};

class BuildinType : public AbstractType
{
public:
    BuildinType(Function f, const String &name);

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    ValuePointer            process(Pointer<ListType> obj);

    virtual ~BuildinType();

private:
    Function m_f;
ATTRIBUTE_READONLY(String, name);
};

class LambdaType : public AbstractType
{
public:
    LambdaType(Pointer<ListType> arg, Pointer<ListType> body, EnvironmentPointer env);
    
    virtual Type            type() const;
    virtual ValuePointer    copy() const final;

    virtual ~LambdaType();

ATTRIBUTE(ListPointer,          Arg,            arg)
ATTRIBUTE(ListPointer,          Body,           body)
ATTRIBUTE(EnvironmentPointer,   Environment,    environment)
};

class MacroType : public LambdaType
{
public:
    MacroType(Pointer<ListType> arg, Pointer<ListType> body, EnvironmentPointer env);

    virtual Type            type() const final;

    virtual ~MacroType();
};

class MapType : public AbstractType
{
public:
    MapType(const Map &m);

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    virtual ~MapType();

ATTRIBUTE(Map, Map, map);
};

#endif // TYPE_H