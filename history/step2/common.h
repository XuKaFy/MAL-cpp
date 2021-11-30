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

enum class Exception {
    EXP_READ_END,
    EXP_READ_MATCH_ERROR,
    EXP_READ_MATCH_MORE,
    EXP_ANA_NONE,
    EXP_ANA_MATCHLESS,
    EXP_ANA_ESCAPE_ERROR,
    EXP_ENV_NOT_FOUND,
    EXP_BUILDIN_FUNCTION_ARG_TYPE_ERROR,
    EXP_BUILDIN_FUNCTION_LENGTH_ERROR,
    EXP_HELPER_CONVERT_NONE,
    EXP_HELPER_APPEND_ERROR,
    EXP_HELPER_GET_ERROR,
    EXP_EVAL_CANNOT_EXECUTE,
};

enum class Type {
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_ATOM,
    TYPE_LIST,
    TYPE_BUILDIN_FUNCTION,
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

class Environment
{
public:
    Environment(Environment *parent = nullptr);

    Environment* parent() const;
    void setParent(Environment *parent);

    AbstractType* getValue(String s);
    void setValue(String s, AbstractType* val);

private:
    Map m_map;
    Environment *m_parent;
};

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

class Helper
{
public:
    static AbstractType* car(ListType* o);
    static AbstractType* cdr(ListType* o);
    static bool atom(AbstractType* o);
    static bool eq(AbstractType* a1, AbstractType* a2);
    static ListType* cons(AbstractType* a1, AbstractType* a2);

    static AbstractType* get(ListType *& o);
    static void next(ListType *& o);

    static bool isEmpty(AbstractType* o);
    static bool isLast(ListType* o);
    static bool isList(ListType* o);
    static bool isSingle(ListType *o);
    static bool isSelfEvaluating(AbstractType* o);
    
    static ListType* append(ListType* o, AbstractType* n);
    static AbstractType* foreach(ListType* o, std::function<void(AbstractType* o)> f);

    template<typename T>
    static T convert(AbstractType* o, Type t) {
        if(o == nullptr)
            throw Exception::EXP_HELPER_CONVERT_NONE;
        if(o->type() != t)
            throw Exception::EXP_BUILDIN_FUNCTION_ARG_TYPE_ERROR;
        return static_cast<T>(o);
    }

    template<typename T>
    static T convert(AbstractType* o) {
        if(o == nullptr)
            throw Exception::EXP_HELPER_CONVERT_NONE;
        return static_cast<T>(o);
    }
};

#endif // COMMON_H