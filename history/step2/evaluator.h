#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "common.h"

class Evaluator
{
public:
    AbstractType* eval(AbstractType* o, Environment* env);

private:
    AbstractType* evalList(ListType* o, Environment* env);
    ListType* listOfValues(ListType* o, Environment* env);
};

#endif // EVALUATOR_H