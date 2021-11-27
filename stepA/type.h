#ifndef TYPE_H
#define TYPE_H

#include <map>
#include <string>
#include <functional>

#include "definition.h"
#include "environment.h"
#include "pointer.h"

#include "debug.h"

class       AbstractType;
class       ListType;

typedef     Pointer<AbstractType>                           ValuePointer;
typedef     Environment<ValuePointer>                       EnvironmentType;
typedef     Pointer<EnvironmentType>                        EnvironmentPointer;
typedef     std::function<ValuePointer(Pointer<ListType>)>  Function;

struct StackFrameType {
    ValuePointer        o;
    EnvironmentPointer  env;
};

class AbstractType
{
public:
    virtual Type            type() const;
    virtual ValuePointer    copy() const;

    virtual ~AbstractType();

private:
    Type m_type;
};

class NumberType : public AbstractType
{
public:
    NumberType(Number n = Number());

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;
    Number                  number() const;
    void                    setNumber(Number n);

    virtual ~NumberType();

private:
    Number m_num;
};

class AtomType : public AbstractType
{
public:
    AtomType(Atom n = Atom());

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;
    Atom                    atom() const;
    void                    setAtom(Atom n);

    virtual ~AtomType();

private:
    Atom m_atom;
};

class ListType : public AbstractType
{
public:
    ListType(ValuePointer first, ValuePointer second);

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    ValuePointer            first() const;
    void                    setFirst(ValuePointer first);
    ValuePointer            second() const;
    void                    setSecond(ValuePointer second);

    virtual ~ListType();

private:
    ValuePointer m_first;
    ValuePointer m_second;
};

class StringType : public AbstractType
{
public:
    StringType(String n = String());

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;
    Atom                    string() const;
    void                    setString(Atom n);

    virtual ~StringType();

private:
    String m_str;
};

class BuildinType : public AbstractType
{
public:
    BuildinType(Function f, String name);

    virtual Type            type() const final;
    virtual ValuePointer    copy() const final;

    ValuePointer            process(Pointer<ListType> obj);
    String                  name() const;

    virtual ~BuildinType();

private:
    Function m_f;
    String m_name;
};

class LambdaType : public AbstractType
{
public:
    LambdaType(Pointer<ListType> arg, Pointer<ListType> body, EnvironmentPointer env);
    
    virtual Type            type() const;
    virtual ValuePointer    copy() const final;

    Pointer<ListType>       arg() const;
    void                    setArg(Pointer<ListType> arg);

    Pointer<ListType>       body() const;
    void                    setBody(Pointer<ListType> body);

    EnvironmentPointer      environment() const;
    void                    setEnvironment(EnvironmentPointer env);

    virtual ~LambdaType();

private:
    Pointer<ListType>  m_arg;
    Pointer<ListType>  m_body;
    EnvironmentPointer m_env;
};

class MacroType : public LambdaType
{
public:
    MacroType(Pointer<ListType> arg, Pointer<ListType> body, EnvironmentPointer env);

    virtual Type type() const final;

    virtual ~MacroType();
};

#endif // TYPE_H