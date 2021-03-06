#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "common.h"
#include "printer.h"
#include "environment.h"

class Evaluator
{
public:
    AbstractType* eval(AbstractType* o, Environment* env);

private:
    AbstractType* eval(AbstractType* o, Environment* env, bool fco, bool root = false);
    AbstractType* evalList(ListType* o, Environment* env, bool fco);

    ListType* listOfValues(ListType* o, Environment* env, bool fco);

    // throw Exception
    AbstractType* funQuote(ListType* o);
    AbstractType* funDef(ListType* o, Environment* env);
    AbstractType* funLambda(ListType* o, Environment* env, bool fco);

    // throw (AbstractType*, Environment*) or Exception
    AbstractType* evalLambda(LambdaType* lam, ListType* args, Environment* env, bool fco);
    AbstractType* funCond(ListType* o, Environment* env, bool fco);
    AbstractType* funLet(ListType* o, Environment* env, bool fco);
    AbstractType* funIf(ListType* o, Environment* env, bool fco);
    AbstractType* funIf2(ListType* o, Environment* env, bool fco);
    AbstractType* funBegin(ListType* o, Environment* env, bool fco);
};

#endif // EVALUATOR_H