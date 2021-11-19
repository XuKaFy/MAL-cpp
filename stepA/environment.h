#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "common.h"
#include "printer.h"

class Environment;

struct StackFrame {
    AbstractType* o;
    Environment* env;
};

class Environment
{
public:
    Environment(Environment *parent = nullptr, LambdaType* lambda = nullptr);

    Environment* parent() const;
    void setParent(Environment *parent);

    AbstractType* getValue(String s) const;
    void setValue(String s, AbstractType* val);
    void setExistValue(String s, AbstractType* val);

    LambdaType* lambda() const;
    void setLambda(LambdaType* lambda);

private:
    Map m_map;
    Environment *m_parent;
    LambdaType* m_lambda;
};

#endif // ENVIRONMENT_H