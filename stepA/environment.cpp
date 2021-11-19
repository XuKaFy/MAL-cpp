#include "environment.h"

Environment::Environment(Environment* parent)
    : m_parent(parent) {
    //printf("Environment Inherit %u -> %u\n", parent, this);
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
    //printf("Environment %u: getValue %s\n", this, s.c_str());
    if(!m_map.count(s)) {
        if(m_parent != nullptr) {
            return m_parent->getValue(s);
        } else {
            throw Exception("Environment::getValue: Can't find value - " + s);
        }
    }
    return m_map[s];
}

void Environment::setValue(String s, AbstractType* val)
{
    //printf("Environment %u: setValue %s -> %s\n", this, s.c_str(), Printer::print(val).c_str());
    m_map[s] = val;
}

void Environment::setExistValue(String s, AbstractType* val)
{
    //printf("Environment %u: setExistValue %s -> %s\n", this, s.c_str(), Printer::print(val).c_str());
    if(!m_map.count(s)) {
        if(m_parent == nullptr) {
            throw Exception("Environment::setExistValue: Can't find value - " + s);
        } else {
            m_parent->setExistValue(s, val);
        }
    } else {
        m_map[s] = val;
    }
}