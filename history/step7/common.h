#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <map>
#include <vector>
#include <functional>

class AbstractType;
class NumberType;
class AtomType;
class ListType;
class LambdaType;
class BuildinFunctionType;

enum class Type {
    TYPE_NUMBER,
    TYPE_STRING,
    TYPE_ATOM,
    TYPE_LIST,
    TYPE_BUILDIN_FUNCTION,
    TYPE_LAMBDA,
    TYPE_NULL,
};

struct Pair {
    Pair(AbstractType* first = nullptr, AbstractType* second = nullptr)
        : first(first), second(second) {}
    AbstractType *first;
    AbstractType *second;
};

typedef     std::string                                 String;
typedef     double                                      Number;
typedef     String                                      Atom;
typedef     Pair                                        List;
typedef     std::map<String, AbstractType*>             Map;
typedef     std::function<AbstractType*(ListType*)>     Function;
typedef     String                                      Exception;

class AbstractType
{
public:
    virtual Type type() const;
    virtual AbstractType* copy() const;

    virtual ~AbstractType();

private:
    Type m_type;
};

class NumberType : public AbstractType
{
public:
    NumberType(Number n = Number());
    virtual Type type() const final;
    virtual AbstractType* copy() const final;
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
    virtual AbstractType* copy() const final;
    Atom atom() const;
    void setAtom(Atom n);

    virtual ~AtomType();

private:
    Atom m_atom;
};

class ListType : public AbstractType
{
public:
    ListType(List n = List());
    virtual Type type() const final;
    virtual AbstractType* copy() const final;
    List list() const;
    void setList(List n);

    virtual ~ListType();

private:
    List m_list;
};

class StringType : public AbstractType
{
public:
    StringType(String n = String());
    virtual Type type() const final;
    virtual AbstractType* copy() const final;
    Atom string() const;
    void setString(Atom n);

    virtual ~StringType();

private:
    String m_str;
};

class BuildinFunctionType : public AbstractType
{
public:
    BuildinFunctionType(Function f, String name);
    virtual Type type() const final;
    virtual AbstractType* copy() const final;

    AbstractType* process(ListType* obj);
    String name() const;

    virtual ~BuildinFunctionType();

private:
    Function m_f;
    String m_name;
};

class LambdaType : public AbstractType
{
public:
    LambdaType(ListType* arg, ListType *body);
    virtual Type type() const final;
    virtual AbstractType* copy() const final;

    ListType* arg() const;
    void setArg(ListType* arg);

    ListType* body() const;
    void setBody(ListType* body);

    virtual ~LambdaType();

private:
    ListType* m_arg;
    ListType* m_body;
};

#endif // COMMON_H