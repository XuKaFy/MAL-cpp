#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <chrono>

#include "type.h"
#include "helper.h"
#include "memory.h"
#include "environment.h"

#include "debug.h"

#ifdef EVAL_DEBUG
#include "printer.h"
#endif

#ifdef EVALLAMBDA_DEBUG
#include "printer.h"
#endif

class Evaluator
{
public:
    ValuePointer evaluate          (ValuePointer o,     EnvironmentPointer env);

private:
    ValuePointer eval          (ValuePointer o,     EnvironmentPointer env,     bool root = false);
    ValuePointer apply         (ListPointer  o,     EnvironmentPointer env);

    ListPointer  listOfValues  (ListPointer  o,     EnvironmentPointer env);

    // throw Exception
    ValuePointer funQuote      (ListPointer  o);
    ValuePointer funDef        (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funSet        (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funDefMacro   (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funTry        (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funLambda     (ListPointer  o,     EnvironmentPointer env);
    ValuePointer funQuasiquote (ValuePointer o,     EnvironmentPointer env);

    // throw StackFrame or Exception
    ValuePointer evalLambda    (Pointer<LambdaType> lam,    ListPointer args, 
                                EnvironmentPointer env,     bool root);
    ValuePointer evalKeyword   (const Keyword &key, ListPointer  l,      EnvironmentPointer env);
    ValuePointer evalHashmap   (const Map &map,     ListPointer  l,      EnvironmentPointer env);

    ValuePointer funCond       (ListPointer o,      EnvironmentPointer env);
    ValuePointer funLet        (ListPointer o,      EnvironmentPointer env);
    ValuePointer funIf         (ListPointer o,      EnvironmentPointer env);
    ValuePointer funIf2        (ListPointer o,      EnvironmentPointer env);
    ValuePointer funBegin      (ListPointer o,      EnvironmentPointer env,     bool root);

    ValuePointer macroExpand   (ValuePointer   o,   EnvironmentPointer env);
    ValuePointer funSwap       (ListPointer    o,   EnvironmentPointer env);
};

#endif // EVALUATOR_H