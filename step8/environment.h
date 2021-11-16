#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "common.h"

class Environment;

struct StackFrame {
    AbstractType* o;
    Environment* env;
};

class Environment
{
public:
    Environment(Environment *parent = nullptr);

    Environment* parent() const;
    void setParent(Environment *parent);

    AbstractType* getValue(String s);
    void setValue(String s, AbstractType* val);

    AbstractType* getFormValue(String s);
    void setFormValue(String s, AbstractType* val);

private:
    Map m_map;
    Map m_form_map;
    Environment *m_parent;
};

#endif // ENVIRONMENT_H