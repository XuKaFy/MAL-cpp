#include "evaluator.h"

AbstractType* Evaluator::eval(AbstractType* o, Environment* env)
{
    return eval(o, env, true, true);
}

AbstractType* Evaluator::eval(AbstractType* o, Environment* env, bool fco, bool root)
{
    //printf("<EVAL> %s\n", Printer::print(o).c_str());
LABEL_AGAIN:
    if(Helper::isSelfEvaluating(o)) {
        //puts("</EVAL>");
        return o;
    }
    if(!root && fco) {
        //puts("</EVAL> Jump out once");
        throw (StackFrame{o, env});
    }
    try {
        switch(o->type()) {
        case Type::TYPE_ATOM:
            return env->getValue(GETATOM(o));
        case Type::TYPE_LIST:
            return apply(GETLIST(o), env, fco);
        default:
            throw Exception("Evaluator::eval: Can't execute");
        }
    } catch (StackFrame s) {
        o = s.o;
        env = s.env;
        goto LABEL_AGAIN;
    }
    //puts("</EVAL>");
    return new AbstractType();
}

AbstractType* Evaluator::apply(ListType* l, Environment* env, bool fco)
{
    if(ISEMPTY(l)) {
        throw Exception("Evaluator::apply: Can't execute");
    }
    if(CAR(l)->type() == Type::TYPE_ATOM) {
        Atom name = GETATOM(CAR(l));
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
        } else if(name == "apply") {
            Helper::next(l);
            return apply(l, env, fco);
        } else if(name == "eval") {
            Helper::next(l);
            SINGLE(it, l);
            it = eval(it, env, true, true); // (eval (list + 1 2 3)) -> (eval '(+ 1 2 3))
            return eval(it, env, fco);
        }
    }
    AbstractType* fun = eval(CAR(l), env, false);
    Helper::next(l);
    switch(fun->type()) {
    case Type::TYPE_BUILDIN_FUNCTION:
        return GETBUILDIN(fun)->process(listOfValues(l, env, fco));
        break;
    case Type::TYPE_LAMBDA:
        return evalLambda(GETLAMBDA(fun), listOfValues(l, env, fco), env, fco);
    default:
        throw Exception("Evaluator::apply: Can't execute");
    }
    return new AbstractType();
}

ListType* Evaluator::listOfValues(ListType* l, Environment *env, bool fco)
{
    if(ISEMPTY(l))
        return l;
    ListType* root = new ListType();
    ListType* current = root;
    AbstractType* remain = FOREACH(o, l, {
        current = Helper::append(current, eval(o, env, false));
    });
    current->setList(List{CAR(current), remain});
    return root;
}

AbstractType* Evaluator::funQuote(ListType* o)
{
    if(!Helper::isSingle(o))
        throw Exception("Evaluator::funQuote: Length of args error");
    return CAR(o);
}

AbstractType* Evaluator::funDef(ListType* o, Environment *env)
{
    AbstractType* a1 = GET(o);
    AbstractType* a2 = funBegin(o, env, false);
    env->setValue(GETATOM(a1), a2);
    return a2;
}

AbstractType* Evaluator::funLambda(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = GET(o);
    if(!Helper::isFlat(GETLIST(a1)))
        throw Exception("Evaluator::funLambda: Not a list");
    return new LambdaType(GETLIST(a1), o);
}

AbstractType* Evaluator::evalLambda(LambdaType* lam, ListType* args, Environment* env, bool fco)
{
    if(!ISEMPTY(args) && !ISLIST(args))
        throw Exception("Evaluator::evalLambda: Args given wrong");
    Environment* childEnv = new Environment(env);
    ListType* lamPointer = lam->arg();
    ListType* argsPointer = args;
    while(!ISEMPTY(lamPointer) && !ISEMPTY(argsPointer)) {
        Atom name = GETATOM(CAR(lamPointer));
        AbstractType* val = CAR(argsPointer);
        childEnv->setValue(name, val);
        Helper::next(lamPointer);
        Helper::next(argsPointer);
    }
    if(!ISEMPTY(lamPointer) || !ISEMPTY(argsPointer))
        throw Exception("Evaluator::evalLambda: Length of Args wrong");
    return funBegin(lam->body(), childEnv, fco); 
}

AbstractType* Evaluator::funCond(ListType* o, Environment *env, bool fco)
{
    if(ISEMPTY(o)) {
        return FALSE;
    }
    if(!ISLIST(o))
        throw Exception("Evaluator::funCond: Not a list");
    while(!ISEMPTY(o)) {
        AbstractType* res = funIf2(GETLIST(CAR(o)), env, fco);
        if(res != nullptr)
            return res;
        Helper::next(o);
    }
    return FALSE;
}

AbstractType* Evaluator::funIf(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = GET(o);
    AbstractType* a2 = GET(o);
    AbstractType* a3 = nullptr;
    if(!ISEMPTY(o)) {
        a3 = GET(o);
        if(!ISEMPTY(o))
            throw Exception("Evaluator::funIf: Length of args error");
    }
    if(ISTRUE(eval(a1, env, false))) {
        return eval(a2, env, fco);
    } else if(a3 != nullptr) {
        return eval(a3, env, fco);
    }
    return FALSE;
}

AbstractType* Evaluator::funIf2(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = GET(o);
    AbstractType* a2 = GET(o);
    if(!ISEMPTY(o)) {
        throw Exception("Evaluator::funIf2: Length of args error");
    }
    if(ISTRUE(eval(a1, env, false))) {
        return eval(a2, env, fco);
    }
    return nullptr;
}

AbstractType* Evaluator::funLet(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = GET(o);
    Environment *childEnv = new Environment(env);
    ListType* args = GETLIST(a1);
    while(!ISEMPTY(args)) {
        ListType* m = GETLIST(GET(args));
        Atom b1 = GETATOM(GET(m));
        AbstractType* b2 = GET(m);
        if(!ISEMPTY(m)) {
            throw Exception("Evaluator::funLet: Length of args error");
        }
        childEnv->setValue(b1, b2);
    }
    return funBegin(o, childEnv, fco); // (let ((x 1) (y 2)) (+ x y))
}

AbstractType* Evaluator::funBegin(ListType* o, Environment *env, bool fco)
{
    if(ISEMPTY(o) || !ISLIST(o))
        throw Exception("Evaluator::funBegin: Args given wrong");
    while(!Helper::isLast(o)) {
        eval(CAR(o), env, true, true);
        Helper::next(o);
    }
    return eval(CAR(o), env, fco);
}