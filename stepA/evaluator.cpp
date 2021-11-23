#include "evaluator.h"

AbstractType* Evaluator::eval(AbstractType* o, Environment* env)
{
    return eval(o, env, true, true);
}

AbstractType* Evaluator::eval(AbstractType* o, Environment* env, bool fco, bool root)
{
    //printf("EVAL<%s>\n", Printer::print(o).c_str());
LABEL_AGAIN:
    if(Helper::isSelfEvaluating(o)) {
        return o;
    }
    if(!root && fco) {
        throw (StackFrame{o, env});
    }
    try {
        AbstractType* res = nullptr;
        switch(o->type()) {
        case Type::TYPE_ATOM:
            res = env->getValue(GETATOM(o));
            break;
        case Type::TYPE_LIST:
            res = apply(GETLIST(o), env, fco);
            break;
        default:
            throw Exception("Evaluator::eval: Can't execute");
        }
        return res;
    } catch (StackFrame s) {
        o = s.o;
        env = s.env;
        goto LABEL_AGAIN;
    }
    return Helper::constantVoid();
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
        } else if(name == "set") {
            Helper::next(l);
            return funSet(l, env);
        } else if(name == "define-macro") {
            Helper::next(l);
            return funDefMacro(l, env);
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
        } else if(name == "quasiquote") {
            Helper::next(l);
            SINGLE(it, l);
            return funQuasiquote(it, env);
        } else if(name == "throw") {
            Helper::next(l);
            SINGLE(it, l);
            throw(eval(it, env, true, true));
        } else if(name == "try") {
            Helper::next(l);
            return funTry(l, env);
        } else if(name == "apply-self") {
            Helper::next(l);
            return funApplySelf(listOfValues(l, env), env, fco);
        }
    }
    AbstractType* fun = eval(CAR(l), env, false);
    Helper::next(l);
    switch(fun->type()) {
    case Type::TYPE_BUILDIN_FUNCTION:
        return GETBUILDIN(fun)->process(listOfValues(l, env));
        break;
    case Type::TYPE_LAMBDA:
        return evalLambda(GETLAMBDA(fun), listOfValues(l, env), env, fco);
    case Type::TYPE_MACRO:
        return eval(evalLambda(GETMACRO(fun), l, env, false), env, fco);
    default:
        throw Exception("Evaluator::apply: Can't execute");
    }
    return Helper::constantVoid();
}

ListType* Evaluator::listOfValues(ListType* l, Environment *env)
{
    if(ISEMPTY(l))
        return l;
    ListType* root = Memory::dispatch(nullptr, nullptr);
    ListType* current = root;
    AbstractType* remain = FOREACH(o, l, {
        current = Helper::append(current, eval(o, env, true, true));
    });
    current->setSecond(remain);
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

AbstractType* Evaluator::funSet(ListType* o, Environment *env)
{
    AbstractType* a1 = GET(o);
    AbstractType* a2 = funBegin(o, env, false);
    env->setExistValue(GETATOM(a1), a2);
    return a2;
}

AbstractType* Evaluator::funDefMacro(ListType* o, Environment *env)
{
    AbstractType* name = GET(o);
    AbstractType* args = GET(o);
    if(!Helper::isFlat(GETLIST(args)))
        throw Exception("Evaluator::funDefMacro: Not a list");
    MacroType* ans = Memory::dispatch(GETLIST(args), o, env, true);
    env->setValue(GETATOM(name), ans);
    return ans;
}

AbstractType* Evaluator::funTry(ListType* o, Environment *env)
{
    AbstractType* tryBody = GET(o);
    ListType* catchList = GETLIST(GET(o));
    if(!ISEMPTY(o))
        throw Exception("Evaluator::funTry: Wrong format");
    Atom catchSymbol = GETATOM(GET(catchList));
    Atom catchName = GETATOM(GET(catchList));
    AbstractType* catchBody = GET(catchList);
    if(!ISEMPTY(catchList) || catchSymbol != "catch")
        throw Exception("Evaluator::funTry: Wrong format");
    try {
        return eval(tryBody, env, true, true);
    } catch(AbstractType* k) {
        Environment* childEnv = Memory::dispatch(env);
        childEnv->setValue(catchName, k);
        return eval(catchBody, childEnv, true, true);
    } catch(Exception e) {
        Environment* childEnv = Memory::dispatch(env);
        childEnv->setValue(catchName, Memory::dispatch(e, true));
        return eval(catchBody, childEnv, true, true);
    }
    return Helper::constantVoid();
}

AbstractType* Evaluator::funLambda(ListType* o, Environment *env, bool fco)
{
    AbstractType* a1 = GET(o);
    if(!Helper::isFlat(GETLIST(a1)))
        throw Exception("Evaluator::funLambda: Not a list");
    return Memory::dispatch(GETLIST(a1), o, env);
}

AbstractType* Evaluator::funApplySelf(ListType* o, Environment* env, bool fco)
{
    if(env->lambda() == nullptr)
        throw Exception("Evaluator::funApplySelf: No self to apply");
    return evalLambda(env->lambda(), o, env, fco);
}

AbstractType* Evaluator::funQuasiquote(AbstractType *o, Environment *env)
{
    if(o->type() != Type::TYPE_LIST) {
        return o;
    }
    ListType* l = GETLIST(o);
    if(ISEMPTY(l)) {
        return l;
    }
    if(CAR(l)->type() == Type::TYPE_ATOM) {
        Atom name = GETATOM(CAR(l));
        if(name == "unquote") {
            Helper::next(l);
            SINGLE(it, l);
            return eval(it, env, false, true);
        } else if(name == "splice-unquote") {
            Helper::next(l);
            SINGLE(it, l);
            ListType* ans = GETLIST(eval(it, env, false, true));
            if(ISEMPTY(ans))
                return FALSE;
            if(!Helper::isList(ans))
                throw Exception("Evaluator::funQuasiquote: args of quasiquote error");
            throw GETLIST(ans->copy());
        }
    }
    ListType* root = Memory::dispatch(nullptr, nullptr);
    ListType* current = root;
    AbstractType* remain = FOREACH(o, l, {
        try {
            current = Helper::append(current, funQuasiquote(o, env));
        } catch (ListType* splice) {
            if(ISEMPTY(root)) {
                delete root;
                root = current = splice;
            } else {
                current->setSecond(splice);
            }
            while(!Helper::isLast(current)) {
                Helper::next(current);
            }
            delete current->second();
            current->setSecond(nullptr);
        }
    });
    current->setSecond(funQuasiquote(remain, env));
    return root;
}

AbstractType* Evaluator::evalLambda(LambdaType* lam, ListType* args, Environment* env, bool fco)
{
    //printf("EVAL LAMBDA\n");
    //printf("    BODY<%s>\n", Printer::print(lam->body()).c_str());
    //printf("    ARGS<%s>\n", Printer::print(lam->arg()).c_str());
    //printf("    GIVEN<%s>\n", Printer::print(args).c_str());
    if(!ISEMPTY(args) && !ISLIST(args))
        throw Exception("Evaluator::evalLambda: Args given wrong");
    Environment* childEnv = Memory::dispatch(lam->environment());
    childEnv->setLambda(lam);
    ListType* lamPointer = lam->arg();
    ListType* argsPointer = args;
    while(!ISEMPTY(lamPointer) && !ISEMPTY(argsPointer)) {
        Atom name = GETATOM(CAR(lamPointer));
        if(name == "&") {
            Helper::next(lamPointer);
            SINGLE(it, lamPointer);
            name = GETATOM(it);
            childEnv->setValue(name, argsPointer);
            return funBegin(lam->body(), childEnv, fco);
        }
        AbstractType* val = CAR(argsPointer);
        childEnv->setValue(name, val);
        Helper::next(lamPointer);
        Helper::next(argsPointer);
    }
    if(!ISEMPTY(lamPointer) && ISEMPTY(argsPointer)) {
        Atom name = GETATOM(CAR(lamPointer));
        if(name == "&") {
            Helper::next(lamPointer);
            SINGLE(it, lamPointer);
            name = GETATOM(it);
            childEnv->setValue(name, FALSE);
            return funBegin(lam->body(), childEnv, fco);
        } else {
            throw Exception("Evaluator::evalLambda: (1) Length of Args wrong");
        }
    }
    if(!ISEMPTY(lamPointer) || !ISEMPTY(argsPointer))
        throw Exception("Evaluator::evalLambda: (2) Length of Args wrong");
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
    Environment *childEnv = Memory::dispatch(env);
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
