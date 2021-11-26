#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "common.h"
#include "printer.h"

class Environment;

struct StackFrame {
    ValueType o;
    Pointer<Environment> env;
};

class Environment
{
public:
    Environment(Pointer<Environment> parent = nullptr, Pointer<LambdaType> lambda = nullptr);
    ~Environment();

    Pointer<Environment> parent() const;
    void setParent(Pointer<Environment> parent);

    ValueType getValue(String s) const;
    void setValue(String s, ValueType val);
    void setExistValue(String s, ValueType val);

    Pointer<LambdaType>  lambda() const;
    void setLambda(Pointer<LambdaType>  lambda);

private:
    Map m_map;
    Pointer<Environment> m_parent;
    Pointer<LambdaType>  m_lambda;
};

#endif // ENVIRONMENT_H