#include "common.h"

Type AbstractType::type() const
{
    return Type::TYPE_NULL;
}

Type NumberType::type() const
{
    return Type::TYPE_NUMBER;
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

AtomType::AtomType(Atom n)
    : m_atom(n) {
}

Type AtomType::type() const
{
    return Type::TYPE_ATOM;
}

Atom AtomType::atom() const
{
    return m_atom;
}

void AtomType::setAtom(Atom n)
{
    m_atom = n;
}

ListType::ListType(List n)
    : m_list(n) {
}

Type ListType::type() const
{
    return Type::TYPE_LIST;
}

List ListType::list() const
{
    return m_list;
}

void ListType::setList(List n)
{
    m_list = n;
}

bool ListType::isEmpty() const
{
    return m_list.first == nullptr && m_list.second == nullptr;
}

bool ListType::isLast() const
{
    if(m_list.second == nullptr)
        return true;
    return m_list.second->type() != Type::TYPE_LIST
            || static_cast<ListType*>(m_list.second)->isEmpty();
}