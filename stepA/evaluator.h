#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "common.h"
#include "helper.h"
#include "reader.h"
#include "printer.h"
#include "environment.h"

class Evaluator
{
public:
    ValueType eval(ValueType o, Pointer<Environment> env);

private:
    ValueType eval(ValueType o, Pointer<Environment> env, bool fco, bool root = false);
    ValueType apply(Pointer<ListType> o, Pointer<Environment> env, bool fco);

    Pointer<ListType> listOfValues(Pointer<ListType> o, Pointer<Environment> env);

    // throw Exception
    ValueType funQuote(Pointer<ListType> o);
    ValueType funDef(Pointer<ListType> o, Pointer<Environment> env);
    ValueType funSet(Pointer<ListType> o, Pointer<Environment> env);
    ValueType funDefMacro(Pointer<ListType> o, Pointer<Environment> env);
    ValueType funTry(Pointer<ListType> o, Pointer<Environment> env);
    ValueType funLambda(Pointer<ListType> o, Pointer<Environment> env, bool fco);
    ValueType funApplySelf(Pointer<ListType> o, Pointer<Environment> env, bool fco);

    ValueType funQuasiquote(ValueType o, Pointer<Environment> env);

    // throw (ValueType, Pointer<Environment>) or Exception
    ValueType evalLambda(Pointer<LambdaType>, Pointer<ListType> args, Pointer<Environment> env, bool fco);
    ValueType funCond(Pointer<ListType> o, Pointer<Environment> env, bool fco);
    ValueType funLet(Pointer<ListType> o, Pointer<Environment> env, bool fco);
    ValueType funIf(Pointer<ListType> o, Pointer<Environment> env, bool fco);
    ValueType funIf2(Pointer<ListType> o, Pointer<Environment> env, bool fco);
    ValueType funBegin(Pointer<ListType> o, Pointer<Environment> env, bool fco);
};

#endif // EVALUATOR_H