#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "type.h"
#include "helper.h"
#include "memory.h"
#include "environment.h"

#include "debug.h"

#ifdef EVAL_DEBUG
#include "printer.h"
#endif

class Evaluator
{
public:
    ValuePointer eval          (ValuePointer o,     EnvironmentPointer env);

private:
    ValuePointer eval          (ValuePointer o,     EnvironmentPointer env,     bool tco,   bool root = false);
    ValuePointer apply         (ListPointer  o,     EnvironmentPointer env,     bool tco);

    ListPointer  listOfValues  (ListPointer  o,     EnvironmentPointer env);

    // throw Exception
    ValuePointer funQuote      (ListPointer  o);
    ValuePointer funDef        (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funSet        (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funDefMacro   (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funTry        (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funLambda     (ListPointer  o,     EnvironmentPointer env,     bool tco);
    ValuePointer funQuasiquote (ValuePointer o,     EnvironmentPointer env);

    // throw StackFrame or Exception
    ValuePointer evalLambda    (Pointer<LambdaType> lam,    ListPointer args, 
                                EnvironmentPointer env,     bool tco);

    ValuePointer funCond       (ListPointer o,      EnvironmentPointer env,     bool tco);
    ValuePointer funLet        (ListPointer o,      EnvironmentPointer env,     bool tco);
    ValuePointer funIf         (ListPointer o,      EnvironmentPointer env,     bool tco);
    ValuePointer funIf2        (ListPointer o,      EnvironmentPointer env,     bool tco);
    ValuePointer funBegin      (ListPointer o,      EnvironmentPointer env,     bool tco);
};

#endif // EVALUATOR_H