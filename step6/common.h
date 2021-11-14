#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <map>
#include <vector>
#include <functional>

#define GETNUMBER(o) (Helper::convert<NumberType*>(o, Type::TYPE_NUMBER)->number())
#define GETSTRING(o) (Helper::convert<StringType*>(o, Type::TYPE_STRING)->string())
#define GETATOM(o) (Helper::convert<AtomType*>(o, Type::TYPE_ATOM)->atom())
#define GETBUILDIN(o) (Helper::convert<BuildinFunctionType*>(o, Type::TYPE_BUILDIN_FUNCTION))
#define GETLAMBDA(o) (Helper::convert<LambdaType*>(o, Type::TYPE_LAMBDA))
#define GETLIST(o) (Helper::convert<ListType*>(o, Type::TYPE_LIST))

#define CAR(o) (Helper::car(o))
#define CDR(o) (Helper::cdr(o))
#define CONS(a1, a2) (Helper::cons(a1, a2))
#define EQ(a1, a2) (Helper::eq(a1, a2))
#define ISEMPTY(o) (Helper::isEmpty(o))
#define ISLIST(o) (Helper::isList(o))
#define ISTRUE(o) (Helper::isTrue(o))
#define ISFALSE(o) (Helper::isFalse(o))

#define GET(o) (Helper::get(o))
#define SINGLE(ans, o) \
    if(Helper::isEmpty(o) || !Helper::isSingle(o)) \
        throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR; \
    AbstractType* ans = CAR(o);
#define DOUBLE(a1, a2, o) \
    AbstractType* a1 = GET(o); \
    AbstractType* a2 = GET(o); \
    if(!Helper::isEmpty(o)) \
        throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
#define NONEARG(o) \
    if(!Helper::isEmpty(o)) \
        throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
#define FUNCTION(o) [](ListType* o) -> AbstractType*
#define QUOTEFUNCTION(o) [&](ListType* o) -> AbstractType*
#define FOREACH(o, m, block) (Helper::foreach(m, [&](AbstractType* o) block))

#define TRUE (Helper::constantTrue())
#define FALSE (Helper::constantFalse())
#define IF(x) ((x) ? TRUE : FALSE)

class AbstractType;
class NumberType;
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
    EXP_EVAL_BUILDIN_LENGTH_ERROR,
    EXP_EVAL_BUILDIN_LIST_ERROR,
    EXP_EVAL_BUILDIN_ARG_ERROR,
};

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

class AbstractType
{
public:
    virtual Type type() const;

private:
    Type m_type;
};

class NumberType : public AbstractType
{
public:
    NumberType(Number n = Number());
    virtual Type type() const final;
    Number number() const;
    void setNumber(Number n);

private:
    Number m_num;
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
    static bool isTrue(AbstractType* o);
    static bool isFalse(AbstractType* o);
    static bool isFlat(ListType* o);

    static AbstractType* constantTrue();
    static AbstractType* constantFalse();
    
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