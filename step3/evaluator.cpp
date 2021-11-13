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
    if(Helper::car(l)->type() == Type::TYPE_ATOM) {
        Atom name = Helper::convert<AtomType*>(Helper::car(l))->atom();
        if(name == "quote") {
            Helper::next(l);
            return funQuote(l);
        } else if(name == "cond") {
            Helper::next(l);
            return funCond(l, env);
        } else if(name == "if") {
            Helper::next(l);
            return funIf(l, env);
        } else if(name == "lambda") {
            Helper::next(l);
            return funLambda(l, env);
        } else if(name == "def!") {
            Helper::next(l);
            return funDef(l, env);
        } else if(name == "let*") {
            Helper::next(l);
            return funLet(l, env);
        }
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
    if(Helper::isEmpty(l))
        return l;
    ListType* root = new ListType();
    ListType* current = root;
    AbstractType* remain = Helper::foreach(l, [&](AbstractType* o) {
        current = Helper::append(current, eval(o, env));
    });
    current->setList(List{Helper::car(current), remain});
    return root;
}

AbstractType* Evaluator::funQuote(ListType* o)
{
    if(!Helper::isSingle(o))
        throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    return Helper::car(o);
}

AbstractType* Evaluator::funCond(ListType* o, Environment *env)
{
    if(Helper::isEmpty(o)) {
        return Helper::constantFalse();
    }
    if(!Helper::isList(o))
        throw Exception::EXP_EVAL_BUILDIN_LIST_ERROR;
    while(!Helper::isEmpty(o)) {
        AbstractType* res = funIf2(Helper::convert<ListType*>(Helper::car(o), Type::TYPE_LIST), env);
        if(res != nullptr)
            return res;
        Helper::next(o);
    }
    return Helper::constantFalse();
}

AbstractType* Evaluator::funIf(ListType* o, Environment *env)
{
    AbstractType* a1 = Helper::get(o);
    AbstractType* a2 = Helper::get(o);
    AbstractType* a3 = nullptr;
    if(!Helper::isEmpty(o)) {
        a3 = Helper::get(o);
        if(!Helper::isEmpty(o))
            throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    }
    if(Helper::isTrue(eval(a1, env))) {
        return eval(a2, env);
    } else if(a3 != nullptr) {
        return eval(a3, env);
    }
    return Helper::constantFalse();
}

AbstractType* Evaluator::funIf2(ListType* o, Environment *env)
{
    AbstractType* a1 = Helper::get(o);
    AbstractType* a2 = Helper::get(o);
    if(!Helper::isEmpty(o)) {
        throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    }
    if(Helper::isTrue(eval(a1, env))) {
        return eval(a2, env);
    }
    return nullptr;
}

AbstractType* Evaluator::funLambda(ListType* o, Environment *env)
{
    AbstractType* a1 = Helper::get(o);
    AbstractType* a2 = Helper::get(o);
    if(!Helper::isEmpty(o)) {
        throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    }
    if(!Helper::isFlat(Helper::convert<ListType*>(a1, Type::TYPE_LIST)))
        throw Exception::EXP_EVAL_BUILDIN_ARG_ERROR;
    return new LambdaType(Helper::convert<ListType*>(a1), a2);
}

AbstractType* Evaluator::funDef(ListType* o, Environment *env)
{
    AbstractType* a1 = Helper::get(o);
    AbstractType* a2 = Helper::get(o);
    if(!Helper::isEmpty(o)) {
        throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    }
    a2 = eval(a2, env);
    env->setValue(Helper::convert<AtomType*>(a1, Type::TYPE_ATOM)->atom(), a2);
    return a2;
}

AbstractType* Evaluator::funLet(ListType* o, Environment *env)
{
    AbstractType* a1 = Helper::get(o);
    AbstractType* a2 = Helper::get(o);
    if(!Helper::isEmpty(o)) {
        throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    }
    Environment *childEnv = new Environment(env);
    ListType* args = Helper::convert<ListType*>(a1, Type::TYPE_LIST);
    while(!Helper::isEmpty(args)) {
        ListType* m = Helper::convert<ListType*>(Helper::get(args), Type::TYPE_LIST);
        AtomType* b1 = Helper::convert<AtomType*>(Helper::get(m));
        AbstractType* b2 = Helper::get(m);
        if(!Helper::isEmpty(m)) {
            throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
        }
        childEnv->setValue(b1->atom(), b2);
    }
    return eval(a2, childEnv); // (let* ((x 1) (y 2)) (+ x y))
}