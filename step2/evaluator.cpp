#include "evaluator.h"

AbstractType* Evaluator::eval(AbstractType* o, Environment* env)
{
    if(Helper::isSelfEvaluating(o))
        return o;
    switch(o->type()) {
    case Type::TYPE_ATOM:
        return eval(env->getValue(Helper::convert<AtomType*>(o)->atom()), env);
    case Type::TYPE_LIST:
        return evalList(Helper::convert<ListType*>(o), env);
    default:
        throw Exception::EXP_EVAL_CANNOT_EXECUTE;
    }
    return new AbstractType();
}

AbstractType* Evaluator::evalList(ListType* l, Environment* env)
{
    if(Helper::isEmpty(l)) {
        throw Exception::EXP_EVAL_CANNOT_EXECUTE;
    }
    AbstractType* fun = eval(Helper::car(l), env);
    Helper::next(l);
    switch(fun->type()) {
    case Type::TYPE_BUILDIN_FUNCTION:
        return Helper::convert<BuildinFunctionType*>(fun, Type::TYPE_BUILDIN_FUNCTION)
            ->process(listOfValues(l, env));
        break;
    case Type::TYPE_LIST:
        // will write
        break;
    default:
        throw Exception::EXP_EVAL_CANNOT_EXECUTE;
    }
    return new AbstractType();
}

ListType* Evaluator::listOfValues(ListType* l, Environment *env)
{
    ListType* root = new ListType();
    ListType* current = root;
    AbstractType* remain = Helper::foreach(l, [&](AbstractType* o) {
        current = Helper::append(current, eval(o, env));
    });
    current->setList(List{Helper::car(current), remain});
    return root;
}