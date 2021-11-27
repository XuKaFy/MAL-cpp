#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>

#include "pointer.h"

#include "debug.h"

#ifdef ENVIRONMENT_DEBUG
#include "printer.h"
#endif

template<typename T>
class Environment
{
public:
    Environment(Pointer<Environment<T>> parent = nullptr)
        : m_parent(parent) {
    #ifdef ENVIRONMENT_DEBUG
        printf("Environment Inherit %u -> %u\n", parent, this);
    #endif
    }

    ~Environment() {
    }

    Pointer<Environment<T>> parent() const {
        return m_parent;   
    }

    void setParent(Pointer<Environment<T>> parent) { 
        m_parent = parent;
    }

    T getValue(String s) const {
#ifdef ENVIRONMENT_DEBUG
    printf("Environment %u: getValue %s\n", this, s.c_str());
#endif
        if(!m_map.count(s)) {
            if(m_parent) {
                return m_parent->getValue(s);
            } else {
                throw Exception("Environment::getValue: Can't find value - " + s);
            }
        }
        return m_map.at(s);
    }

    void setValue(String s, const T &val) {
#ifdef ENVIRONMENT_DEBUG
        printf("Environment %u: setValue %s -> %s\n", this, s.c_str(), Printer::print(val).c_str());
#endif
        m_map[s] = val;
    }

    void setExistValue(String s, const T &val) {
#ifdef ENVIRONMENT_DEBUG
        printf("Environment %u: setExistValue %s -> %s\n", this, s.c_str(), Printer::print(val).c_str());
#endif
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

private:
    std::map<String, T>        m_map;
    Pointer<Environment<T>>    m_parent;
};

#endif // ENVIRONMENT_H