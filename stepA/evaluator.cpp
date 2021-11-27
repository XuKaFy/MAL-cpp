#include "evaluator.h"

ValuePointer Evaluator::eval(ValuePointer o, EnvironmentPointer env)
{
    return eval(o, env, true, true);
}

ValuePointer Evaluator::eval(ValuePointer o, EnvironmentPointer env, bool tco, bool root)
{
#ifdef EVAL_DEBUG
    printf("EVAL<%s>\n", Printer::print(o).c_str());
#endif
LABEL_AGAIN:
    if(Helper::isSelfEvaluating(o)) {
        return o;
    }
    if(!root && tco) {
        throw (StackFrame{o, env});
    }
    try {
        ValuePointer res = nullptr;
        switch(o->type()) {
        case Type::TYPE_ATOM:
            res = env->getValue(GETATOM(o));
            break;
        case Type::TYPE_LIST:
            res = apply(GETLIST(o), env, tco);
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
    return VOID;
}

ValuePointer Evaluator::apply(ListPointer l, EnvironmentPointer env, bool tco)
{
    if(ISEMPTY(l))
        throw Exception("Evaluator::apply: Can't execute");
    if(CAR(l)->type() == Type::TYPE_ATOM) {
        Atom name = GETATOM(CAR(l));
        if(name == "quote") {
            NEXT(l);
            return funQuote(l);
        } else if(name == "cond") {
            NEXT(l);
            return funCond(l, env, tco);
        } else if(name == "if") {
            NEXT(l);
            return funIf(l, env, tco);
        } else if(name == "lambda") {
            NEXT(l);
            return funLambda(l, env, tco);
        } else if(name == "define") {
            NEXT(l);
            return funDef(l, env);
        } else if(name == "set") {
            NEXT(l);
            return funSet(l, env);
        } else if(name == "define-macro") {
            NEXT(l);
            return funDefMacro(l, env);
        } else if(name == "let") {
            NEXT(l);
            return funLet(l, env, tco);
        } else if(name == "begin") {
            NEXT(l);
            return funBegin(l, env, tco);
        } else if(name == "apply") {
            NEXT(l);
            return apply(l, env, tco);
        } else if(name == "eval") {
            NEXT(l);
            SINGLE(it, l);
            it = eval(it, env, true, true); // (eval (list + 1 2 3)) -> (eval '(+ 1 2 3))
            return eval(it, env, tco);
        } else if(name == "quasiquote") {
            NEXT(l);
            SINGLE(it, l);
            return funQuasiquote(it, env);
        } else if(name == "throw") {
            NEXT(l);
            SINGLE(it, l);
            throw(eval(it, env, true, true));
        } else if(name == "try") {
            NEXT(l);
            return funTry(l, env);
        }
    }
    ValuePointer fun = eval(CAR(l), env, false);
    NEXT(l);
    switch(fun->type()) {
    case Type::TYPE_BUILDIN:
        return GETBUILDIN(fun)->process(listOfValues(l, env));
        break;
    case Type::TYPE_LAMBDA:
        return evalLambda(GETLAMBDA(fun), listOfValues(l, env), env, tco);
    case Type::TYPE_MACRO:
        return eval(evalLambda(GETMACRO(fun).convert<LambdaType>(), l, env, false), env, tco);
    default:
        throw Exception("Evaluator::apply: Can't execute");
    }
    return VOID;
}

ListPointer Evaluator::listOfValues(ListPointer l, EnvironmentPointer env)
{
    if(ISEMPTY(l))
        return l;
    ListPointer root = Memory::dispatchList();
    ListPointer current = root;
    ValuePointer remain = FOREACH(o, l, {
        current = Helper::append(current, eval(o, env, true, true));
    });
    current->setSecond(remain);
    return root;
}

ValuePointer Evaluator::funQuote(ListPointer o)
{
    if(!Helper::isSingle(o))
        throw Exception("Evaluator::funQuote: Length of args error");
    return CAR(o);
}

ValuePointer Evaluator::funDef(ListPointer o, EnvironmentPointer env)
{
    ValuePointer a1 = GET(o);
    ValuePointer a2 = funBegin(o, env, false);
    env->setValue(GETATOM(a1), a2);
    return a2;
}

ValuePointer Evaluator::funSet(ListPointer o, EnvironmentPointer env)
{
    ValuePointer a1 = GET(o);
    ValuePointer a2 = funBegin(o, env, false);
    env->setExistValue(GETATOM(a1), a2);
    return a2;
}

ValuePointer Evaluator::funDefMacro(ListPointer o, EnvironmentPointer env)
{
    ValuePointer name = GET(o);
    ValuePointer args = GET(o);
    if(!Helper::isFlat(GETLIST(args)))
        throw Exception("Evaluator::funDefMacro: Not a list");
    ValuePointer ans = VALUE(Memory::dispatchMacro(GETLIST(args), o, env));
    env->setValue(GETATOM(name), ans);
    return ans;
}

ValuePointer Evaluator::funTry(ListPointer o, EnvironmentPointer env)
{
    DOUBLE(tryBody, catchListValue, o)
    TRIBLE_FROM_VALUE(catchSymbol, catchName, catchBody, catchListValue)
    if(GETATOM(catchSymbol) != "catch")
        throw Exception("Evaluator::funTry: Wrong format");
    try {
        return eval(tryBody, env, true, true);
    } catch(ValuePointer k) {
        EnvironmentPointer childEnv = Memory::dispatchEnvironment(env);
        childEnv->setValue(GETATOM(catchName), k);
        return eval(catchBody, childEnv, true, true);
    } catch(Exception e) {
        EnvironmentPointer childEnv = Memory::dispatchEnvironment(env);
        childEnv->setValue(GETATOM(catchName), VALUE(Memory::dispatchString(e)));
        return eval(catchBody, childEnv, true, true);
    }
    return VOID;
}

ValuePointer Evaluator::funLambda(ListPointer o, EnvironmentPointer env, bool tco)
{
    ValuePointer a1 = GET(o);
    if(!Helper::isFlat(GETLIST(a1)))
        throw Exception("Evaluator::funLambda: Not a list");
    return VALUE(Memory::dispatchLambda(GETLIST(a1), o, env));
}

ValuePointer Evaluator::funQuasiquote(ValuePointer o, EnvironmentPointer env)
{
    if(o->type() != Type::TYPE_LIST) {
        return o;
    }
    ListPointer l = GETLIST(o);
    if(ISEMPTY(l))
        return FALSE;
    if(CAR(l)->type() == Type::TYPE_ATOM) {
        Atom name = GETATOM(CAR(l));
        if(name == "unquote") {
            NEXT(l);
            SINGLE(it, l);
            return eval(it, env, false, true);
        } else if(name == "splice-unquote") {
            NEXT(l);
            SINGLE(it, l);
            ListPointer ans = GETLIST(eval(it, env, false, true));
            if(ISEMPTY(ans))
                return FALSE;
            if(!ISLIST(ans))
                throw Exception("Evaluator::funQuasiquote: args of quasiquote error");
            throw GETLIST(ans->copy());
        }
    }
    ListPointer root = Memory::dispatchList();
    ListPointer current = root;
    ValuePointer remain = FOREACH(o, l, {
        try {
            current = Helper::append(current, funQuasiquote(o, env));
        } catch (ListPointer splice) {
            if(ISEMPTY(root)) {
                root = current = splice;
            } else {
                current->setSecond(VALUE(splice));
            }
            while(!Helper::isLast(current)) {
                NEXT(current);
            }
            current->setSecond(nullptr);
        }
    });
    current->setSecond(funQuasiquote(remain, env));
    return VALUE(root);
}

ValuePointer Evaluator::evalLambda(Pointer<LambdaType> lam, ListPointer args, EnvironmentPointer env, bool tco)
{
#ifdef EVALLAMBDA_DEBUG
    printf("EVAL LAMBDA\n");
    printf("    BODY<%s>\n", Printer::castList(lam->body()).c_str());
    printf("    ARGS<%s>\n", Printer::castList(lam->arg()).c_str());
    printf("    GIVEN<%s>\n", Printer::castList(args).c_str());
#endif
    if(!ISEMPTY(args) && !ISLIST(args))
        throw Exception("Evaluator::evalLambda: Args given wrong");
    EnvironmentPointer childEnv = Memory::dispatchEnvironment(lam->environment());
    ListPointer lamPointer = lam->arg();
    ListPointer argsPointer = args;
    while(!ISEMPTY(lamPointer)) {
        Atom name = GETATOM(CAR(lamPointer));
        if(name == "&") {
            NEXT(lamPointer);
            SINGLE(it, lamPointer);
            name = GETATOM(it);
            childEnv->setValue(name, VALUE(argsPointer));
            return funBegin(lam->body(), childEnv, tco);
        }
        if(ISEMPTY(argsPointer))
            throw Exception("Evaluator::evalLambda: (1) Length of Args wrong");
        ValuePointer val = CAR(argsPointer);
        childEnv->setValue(name, val);
        NEXT(lamPointer);
        NEXT(argsPointer);
    }
    if(!ISEMPTY(argsPointer))
        throw Exception("Evaluator::evalLambda: (2) Length of Args wrong");
    return funBegin(lam->body(), childEnv, tco); 
}

ValuePointer Evaluator::funCond(ListPointer o, EnvironmentPointer env, bool tco)
{
    if(ISEMPTY(o))
        return FALSE;
    if(!ISLIST(o))
        throw Exception("Evaluator::funCond: Not a list");
    while(!ISEMPTY(o)) {
        ValuePointer res = funIf2(GETLIST(CAR(o)), env, tco);
        if(res)
            return res;
        NEXT(o);
    }
    return FALSE;
}

ValuePointer Evaluator::funIf(ListPointer o, EnvironmentPointer env, bool tco)
{
    ValuePointer a1 = GET(o);
    ValuePointer a2 = GET(o);
    ValuePointer a3;
    if(!ISEMPTY(o)) {
        a3 = GET(o);
        if(!ISEMPTY(o))
            throw Exception("Evaluator::funIf: Length of args error");
    }
    if(ISTRUE(eval(a1, env, false))) {
        return eval(a2, env, tco);
    } else if(a3) {
        return eval(a3, env, tco);
    }
    return FALSE;
}

ValuePointer Evaluator::funIf2(ListPointer o, EnvironmentPointer env, bool tco)
{
    ValuePointer a1 = GET(o);
    ValuePointer a2 = GET(o);
    if(!ISEMPTY(o))
        throw Exception("Evaluator::funIf2: Length of args error");
    if(ISTRUE(eval(a1, env, false)))
        return eval(a2, env, tco);
    return nullptr;
}

ValuePointer Evaluator::funLet(ListPointer o, EnvironmentPointer env, bool tco)
{
    ListPointer args = GETLIST(GET(o));
    if(!ISLIST(args))
        throw Exception("Evaluator::funLet: Not a list");
    EnvironmentPointer childEnv = Memory::dispatchEnvironment(env);
    FOREACH(m, args, {
        DOUBLE_FROM_VALUE(b1, b2, m)
        childEnv->setValue(GETATOM(b1), b2);
    });
    return funBegin(o, childEnv, tco); // (let ((x 1) (y 2)) (+ x y))
}

ValuePointer Evaluator::funBegin(ListPointer o, EnvironmentPointer env, bool tco)
{
    if(ISEMPTY(o) || !ISLIST(o))
        throw Exception("Evaluator::funBegin: Args given wrong");
    while(!Helper::isLast(o)) {
        eval(CAR(o), env, true, true);
        NEXT(o);
    }
    return eval(CAR(o), env, tco);
}
