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
    ValuePointer eval  (ValuePointer o,           EnvironmentPointer env);

private:
    ValuePointer eval  (ValuePointer o,           EnvironmentPointer env, bool tco, bool root = false);
    ValuePointer apply (Pointer<ListType> o,      EnvironmentPointer env, bool tco);

    Pointer<ListType> listOfValues(Pointer<ListType> o, EnvironmentPointer env);

    // throw Exception
    ValuePointer funQuote      (Pointer<ListType> o);
    ValuePointer funDef        (Pointer<ListType> o, EnvironmentPointer env);
    ValuePointer funSet        (Pointer<ListType> o, EnvironmentPointer env);
    ValuePointer funDefMacro   (Pointer<ListType> o, EnvironmentPointer env);
    ValuePointer funTry        (Pointer<ListType> o, EnvironmentPointer env);
    ValuePointer funLambda     (Pointer<ListType> o, EnvironmentPointer env, bool tco);

    ValuePointer funQuasiquote (ValuePointer o, EnvironmentPointer env);

    // throw (ValuePointer, EnvironmentPointer) or Exception
    ValuePointer evalLambda    (Pointer<LambdaType>, Pointer<ListType> args, 
                                EnvironmentPointer env, bool tco);
    ValuePointer funCond       (Pointer<ListType> o, EnvironmentPointer env, bool tco);
    ValuePointer funLet        (Pointer<ListType> o, EnvironmentPointer env, bool tco);
    ValuePointer funIf         (Pointer<ListType> o, EnvironmentPointer env, bool tco);
    ValuePointer funIf2        (Pointer<ListType> o, EnvironmentPointer env, bool tco);
    ValuePointer funBegin      (Pointer<ListType> o, EnvironmentPointer env, bool tco);
};

#endif // EVALUATOR_H