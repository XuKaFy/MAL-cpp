#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <map>
#include <vector>
#include <functional>

class AbstractType;
class FloatType;
class AtomType;
class ListType;
class LambdaType;
class BuildinFunctionType;

enum class Type {
    TYPE_FLOAT,
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
typedef     double                                      Float;
typedef     String                                      Atom;
typedef     Pair                                        List;
typedef     std::map<String, AbstractType*>             Map;
typedef     std::function<AbstractType*(ListType*)>     Function;
typedef     String                                      Exception;

class AbstractType
{
public:
    virtual Type type() const;

private:
    Type m_type;
};

class FloatType : public AbstractType
{
public:
    FloatType(Float n = Float());
    virtual Type type() const final;
    Float number() const;
    void setFloat(Float n);

private:
    Float m_num;
};

class AtomType : public AbstractType
{
public:
    AtomType(Atom n = Atom());
    virtual Type type() const final;
    Atom atom() const;
    void setAtom(Atom n);

private:
    Atom m_atom;
};

class ListType : public AbstractType
{
public:
    ListType(List n = List());
    virtual Type type() const final;
    List list() const;
    void setList(List n);

private:
    List m_list;
};

class StringType : public AbstractType
{
public:
    StringType(String n = String());
    virtual Type type() const final;
    Atom string() const;
    void setString(Atom n);

private:
    String m_str;
};

class BuildinFunctionType : public AbstractType
{
public:
    BuildinFunctionType(Function f, String name);
    virtual Type type() const final;

    AbstractType* process(ListType* obj);
    String name() const;

private:
    Function m_f;
    String m_name;
};

class LambdaType : public AbstractType
{
public:
    LambdaType(ListType* arg, ListType *body);
    virtual Type type() const final;

    ListType* arg() const;
    void setArg(ListType* arg);

    ListType* body() const;
    void setBody(ListType* body);

private:
    ListType* m_arg;
    ListType* m_body;
};

#endif // COMMON_H