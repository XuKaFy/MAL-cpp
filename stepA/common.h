#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>

#include "definition.h"
#include "debug.h"
#include "pointer.h"

class AbstractType
{
public:
    virtual Type type() const;
    virtual ValueType copy() const;

    virtual ~AbstractType();

private:
    Type m_type;
};

class NumberType : public AbstractType
{
public:
    NumberType(Number n = Number());
    virtual Type type() const final;
    virtual ValueType copy() const final;
    Number number() const;
    void setNumber(Number n);

    virtual ~NumberType();

private:
    Number m_num;
};

class AtomType : public AbstractType
{
public:
    AtomType(Atom n = Atom());
    virtual Type type() const final;
    virtual ValueType copy() const final;
    Atom atom() const;
    void setAtom(Atom n);

    virtual ~AtomType();

private:
    Atom m_atom;
};

class ListType : public AbstractType
{
public:
    ListType(ValueType first, ValueType second);
    virtual Type type() const final;
    virtual ValueType copy() const final;

    ValueType first() const;
    void setFirst(ValueType first);
    ValueType second() const;
    void setSecond(ValueType second);

    virtual ~ListType();

private:
    ValueType m_first;
    ValueType m_second;
};

class StringType : public AbstractType
{
public:
    StringType(String n = String());
    virtual Type type() const final;
    virtual ValueType copy() const final;
    Atom string() const;
    void setString(Atom n);

    virtual ~StringType();

private:
    String m_str;
};

class BuildinType : public AbstractType
{
public:
    BuildinType(Function f, String name);
    virtual Type type() const final;
    virtual ValueType copy() const final;

    ValueType process(Pointer<ListType> obj);
    String name() const;

    virtual ~BuildinType();

private:
    Function m_f;
    String m_name;
};

class LambdaType : public AbstractType
{
public:
    LambdaType(Pointer<ListType> arg, Pointer<ListType> body, Pointer<Environment> env);
    virtual Type type() const;
    virtual ValueType copy() const final;

    Pointer<ListType> arg() const;
    void setArg(Pointer<ListType> arg);

    Pointer<ListType> body() const;
    void setBody(Pointer<ListType> body);

    Pointer<Environment> environment() const;
    void setEnvironment(Pointer<Environment> env);

    virtual ~LambdaType();

private:
    Pointer<ListType> m_arg;
    Pointer<ListType> m_body;
    Pointer<Environment> m_env;
};

class MacroType : public LambdaType
{
public:
    MacroType(Pointer<ListType> arg, Pointer<ListType> body, Pointer<Environment> env);
    virtual Type type() const final;

    virtual ~MacroType();
};

#endif // COMMON_H