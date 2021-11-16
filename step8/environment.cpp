#include "environment.h"

Environment::Environment(Environment* parent)
    : m_parent(parent) {
    ;
}

Environment* Environment::parent() const
{
    return m_parent;
}

void Environment::setParent(Environment* parent)
{
    m_parent = parent;
}

AbstractType* Environment::getValue(String s)
{
    if(!m_map.count(s)) {
        if(parent() != nullptr) {
            return parent()->getValue(s);
        } else {
            throw Exception("Environment::getValue: Can't find value");
        }
    }
    return m_map[s];
}

void Environment::setValue(String s, AbstractType* val)
{
    m_map[s] = val;
}

AbstractType* Environment::getFormValue(String s)
{
    if(!m_form_map.count(s)) {
        if(parent() != nullptr) {
            return parent()->getFormValue(s);
        } else {
            throw Exception("Environment::getFormValue: Can't find value");
        }
    }
    return m_form_map[s];
}

void Environment::setFormValue(String s, AbstractType* val)
{
    m_form_map[s] = val;
}