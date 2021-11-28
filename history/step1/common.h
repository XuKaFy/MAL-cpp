#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>

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
};

enum class TokenType {
    TOKEN_OPERATOR,
    TOKEN_NUMBER,
    TOKEN_ATOM,
    TOKEN_STRING,
};

enum class Type {
    TYPE_NUMBER,
    TYPE_STRING,
    TYPE_ATOM,
    TYPE_LIST,
    TYPE_NULL,
};

struct Pair {
    Pair(AbstractType* first = nullptr, AbstractType* second = nullptr)
        : first(first), second(second) {}
    AbstractType *first;
    AbstractType *second;
};

typedef     std::string         String;
typedef     double              Number;
typedef     String              Atom;
typedef     Pair                List;

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

    bool isEmpty() const;
    bool isLast() const;

private:
    List m_list;
};

#endif // COMMON_H