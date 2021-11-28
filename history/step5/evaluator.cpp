#include "evaluator.h"

AbstractType* Evaluator::eval(AbstractType* o, Environment* env)
{
    return eval(o, env, true, true);
}

AbstractType* Evaluator::eval(AbstractType* o, Environment* env, bool fco, bool root)
{
    // (define sum (lambda (x) (if (empty? x) 0 (+ (car x) (sum (cdr x))))))
    // (sum (list 1 2 3))
    // (begin (begin (+ 1 2) (* 3 4)) (begin (+ 5 6) (* 7 8)))
    //printf("<EVAL> %s\n", Printer::print(o).c_str());
LABEL_AGAIN:
    if(Helper::isSelfEvaluating(o))
        return o;
    if(!root && fco) {
        //puts("</EVAL> Jump out once");
        throw (StackFrame{o, env});
    }
    try {
        switch(o->type()) {
        case Type::TYPE_ATOM:
            return env->getValue(Helper::convert<AtomType*>(o)->atom());
        case Type::TYPE_LIST:
            return evalList(Helper::convert<ListType*>(o), env, fco);
        default:
            throw Exception::EXP_EVAL_CANNOT_EXECUTE;
        }
    } catch (StackFrame s) {
        o = s.o;
        env = s.env;
        goto LABEL_AGAIN;
    }
    //puts("</EVAL>");
    return new AbstractType();
}

AbstractType* Evaluator::evalList(ListType* l, Environment* env, bool fco)
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
            return funCond(l, env, fco);
        } else if(name == "if") {
            Helper::next(l);
            return funIf(l, env, fco);
        } else if(name == "lambda") {
            Helper::next(l);
            return funLambda(l, env, fco);
        } else if(name == "define") {
            Helper::next(l);
            return funDef(l, env);
        } else if(name == "let") {
            Helper::next(l);
            return funLet(l, env, fco);
        } else if(name == "begin") {
            Helper::next(l);
            return funBegin(l, env, fco);
        }
    }
    AbstractType* fun = eval(Helper::car(l), env, false);
    Helper::next(l);
    switch(fun->type()) {
    case Type::TYPE_BUILDIN_FUNCTION:
        return Helper::convert<BuildinFunctionType*>(fun)
            ->process(listOfValues(l, env, fco));
        break;
    case Type::TYPE_LAMBDA:
        return evalLambda(Helper::convert<LambdaType*>(fun), listOfValues(l, env, fco), env, fco);
    default:
        throw Exception::EXP_EVAL_CANNOT_EXECUTE;
    }
    return new AbstractType();
}

ListType* Evaluator::listOfValues(ListType* l, Environment *env, bool fco)
{
    if(Helper::isEmpty(l))
        return l;
    ListType* root = new ListType();
    ListType* current = root;
    AbstractType* remain = Helper::foreach(l, [&](AbstractType* o) {
        current = Helper::append(current, eval(o, env, false));
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

AbstractType* Evaluator::funDef(ListType* o, Environment *env)
{
    AbstractType* a1 = Helper::get(o);
    AbstractType* a2 = funBegin(o, env, false);
    env->setValue(Helper::convert<AtomType*>(a1, Type::TYPE_ATOM)->atom(), a2);
    return a2;
    // (define sqr (lambda (x) (* x x)))
    // (sqr 50)
    // (define f (lambda (x) (println x) (if (> x 0) (f (- x 1)) (quote end)))) 
}

AbstractType* Evaluator::funLambda(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = Helper::get(o);
    if(!Helper::isFlat(Helper::convert<ListType*>(a1, Type::TYPE_LIST)))
        throw Exception::EXP_EVAL_BUILDIN_ARG_ERROR;
    return new LambdaType(Helper::convert<ListType*>(a1), o);
}

AbstractType* Evaluator::evalLambda(LambdaType* lam, ListType* args, Environment* env, bool fco)
{
    Environment* childEnv = new Environment(env);
    if(!Helper::isList(args) && !Helper::isEmpty(args))
        throw Exception::EXP_EVAL_BUILDIN_LIST_ERROR;
    ListType* lamPointer = lam->arg();
    ListType* argsPointer = args;
    while(!Helper::isEmpty(lamPointer) && !Helper::isEmpty(argsPointer)) {
        Atom name = Helper::convert<AtomType*>(Helper::car(lamPointer))->atom();
        AbstractType* val = Helper::car(argsPointer);
        childEnv->setValue(name, val);
        Helper::next(lamPointer);
        Helper::next(argsPointer);
    }
    if(!Helper::isEmpty(lamPointer) || !Helper::isEmpty(argsPointer))
        throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    return funBegin(lam->body(), childEnv, fco); 
    // ((lambda (x y) (+ (* x x) (* y y))) 2 6)
    // ((lambda () (+ (* 2 2) (* 3 3))))
}

AbstractType* Evaluator::funCond(ListType* o, Environment *env, bool fco)
{
    if(Helper::isEmpty(o)) {
        return Helper::constantFalse();
    }
    if(!Helper::isList(o))
        throw Exception::EXP_EVAL_BUILDIN_LIST_ERROR;
    while(!Helper::isEmpty(o)) {
        AbstractType* res = funIf2(Helper::convert<ListType*>(Helper::car(o), Type::TYPE_LIST), env, fco);
        if(res != nullptr)
            return res;
        Helper::next(o);
    }
    return Helper::constantFalse();
}

AbstractType* Evaluator::funIf(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = Helper::get(o);
    AbstractType* a2 = Helper::get(o);
    AbstractType* a3 = nullptr;
    if(!Helper::isEmpty(o)) {
        a3 = Helper::get(o);
        if(!Helper::isEmpty(o))
            throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    }
    if(Helper::isTrue(eval(a1, env, false))) {
        return eval(a2, env, fco);
    } else if(a3 != nullptr) {
        return eval(a3, env, fco);
    }
    return Helper::constantFalse();
}

AbstractType* Evaluator::funIf2(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = Helper::get(o);
    AbstractType* a2 = Helper::get(o);
    if(!Helper::isEmpty(o)) {
        throw Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR;
    }
    if(Helper::isTrue(eval(a1, env, false))) {
        return eval(a2, env, fco);
    }
    return nullptr;
}

AbstractType* Evaluator::funLet(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = Helper::get(o);
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
    return funBegin(o, childEnv, fco); // (let ((x 1) (y 2)) (+ x y))
}

AbstractType* Evaluator::funBegin(ListType* o, Environment *env, bool fco)
{
    if(Helper::isEmpty(o) || !Helper::isList(o))
        throw Exception::EXP_EVAL_BUILDIN_LIST_ERROR;
    while(!Helper::isLast(o)) {
        eval(Helper::car(o), env, true, true);
        Helper::next(o);
    }
    return eval(Helper::car(o), env, fco);
}