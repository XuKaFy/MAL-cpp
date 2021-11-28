#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "common.h"
#include "environment.h"

class Evaluator
{
public:
    AbstractType* eval(AbstractType* o, Environment* env);

private:
    AbstractType* evalList(ListType* o, Environment* env);
    AbstractType* evalLambda(LambdaType* lam, ListType* args, Environment* env);

    ListType* listOfValues(ListType* o, Environment* env);

    AbstractType* funQuote(ListType* o);
    AbstractType* funCond(ListType* o, Environment* env);

    AbstractType* funLambda(ListType* o, Environment* env);
    AbstractType* funDef(ListType* o, Environment* env);
    AbstractType* funLet(ListType* o, Environment* env);
    AbstractType* funIf(ListType* o, Environment* env);
    AbstractType* funIf2(ListType* o, Environment* env);
};

#endif // EVALUATOR_H