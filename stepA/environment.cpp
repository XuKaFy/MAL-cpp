#include "environment.h"

Environment::Environment(Pointer<Environment> parent, Pointer<LambdaType>  lambda)
    : m_parent(parent), m_lambda(lambda) {
    //printf("Environment Inherit %u -> %u\n", parent, this);
    if(m_parent && m_lambda)
        m_lambda = m_parent->lambda();
}

Environment::~Environment()
{
    ;
}

Pointer<Environment> Environment::parent() const
{
    return m_parent;
}

void Environment::setParent(Pointer<Environment> parent)
{
    m_parent = parent;
}

ValueType Environment::getValue(String s) const
{
    //printf("Environment %u: getValue %s\n", this, s.c_str());
    if(!m_map.count(s)) {
        if(m_parent) {
            return m_parent->getValue(s);
        } else {
            throw Exception("Environment::getValue: Can't find value - " + s);
        }
    }
    return m_map.at(s);
}

void Environment::setValue(String s, ValueType val)
{
    //printf("Environment %u: setValue %s -> %s\n", this, s.c_str(), Printer::print(val).c_str());
    m_map[s] = val;
}

void Environment::setExistValue(String s, ValueType val)
{
    //printf("Environment %u: setExistValue %s -> %s\n", this, s.c_str(), Printer::print(val).c_str());
    if(!m_map.count(s)) {
        if(!m_parent) {
            throw Exception("Environment::setExistValue: Can't find value - " + s);
        } else {
            m_parent->setExistValue(s, val);
        }
    } else {
        m_map[s] = val;
    }
}

Pointer<LambdaType>  Environment::lambda() const
{
    return m_lambda;
}

void Environment::setLambda(Pointer<LambdaType>  lambda)
{
    m_lambda = lambda;
}