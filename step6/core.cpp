#include "core.h"

void Core::registerBasicFunction(Environment* env)
{
    registerFunction(env, "+", FUNCTION(o) {
        Number num = 0;
        FOREACH(m, o, { num += GETNUMBER(m); });
        return new NumberType(num);
    });
    registerFunction(env, "*", FUNCTION(o) {
        Number num = 1;
        FOREACH(m, o, { num *= GETNUMBER(m); });
        return new NumberType(num);
    });
    registerFunction(env, "-", FUNCTION(o) {
        Number num = GETNUMBER(GET(o));
        FOREACH(m, o, { num -= GETNUMBER(m); });
        return new NumberType(num);
    });
    registerFunction(env, "/", FUNCTION(o) {
        Number num = GETNUMBER(GET(o));
        FOREACH(m, o, { num /= GETNUMBER(m); });
        return new NumberType(num);
    });
    registerFunction(env, "car", FUNCTION(o) {
        SINGLE(it, o);
        return CAR(GETLIST(it));
    });
    registerFunction(env, "cdr", FUNCTION(o) {
        SINGLE(it, o);
        return CDR(GETLIST(it));
    });
    registerFunction(env, "atom", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_ATOM);
    });
    registerFunction(env, "cons", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return CONS(a1, a2);
    });
    registerFunction(env, "eq", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(EQ(a1, a2));
    });
    registerFunction(env, "list", FUNCTION(o) {
        return o;
    });
    registerFunction(env, "empty?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(ISEMPTY(it));
    });
    registerFunction(env, "list?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_LIST 
                && ISLIST((Helper::convert<ListType*>(o))));
    });
    registerFunction(env, "true?", FUNCTION(o) {
        SINGLE(a1, o);
        return IF(ISTRUE(a1));
    });
    registerFunction(env, "false?", FUNCTION(o) {
        SINGLE(a1, o);
        return IF(ISFALSE(a1));
    });
    registerFunction(env, ">", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETNUMBER(a1) > GETNUMBER(a2));
    });
    registerFunction(env, "<", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETNUMBER(a1) < GETNUMBER(a2));
    });
    registerFunction(env, "<=", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETNUMBER(a1) <= GETNUMBER(a2));
    });
    registerFunction(env, ">=", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETNUMBER(a1) >= GETNUMBER(a2));
    });
    registerFunction(env, "print", FUNCTION(o) {
        FOREACH(m, o, { std::cout << Printer::print(m); });
        return new AbstractType();
    });
    registerFunction(env, "newline", FUNCTION(o) {
        NONEARG(o)
        std::cout << std::endl;
        return new AbstractType();
    });
    registerFunction(env, "read", FUNCTION(o) {
        NONEARG(o)
        String s;
        getline(std::cin, s);
        return Reader::read(s);
    });
    registerFunction(env, "read-string", FUNCTION(o) {
        NONEARG(o)
        String s;
        getline(std::cin, s);
        return new StringType(s);
    });
    registerFunction(env, "print-string", FUNCTION(o) {
        FOREACH(m, o, { std::cout << Printer::printWithEscape(GETSTRING(m)); });
        return new AbstractType();
    });
}

void Core::registerFunction(Environment *env, String name, Function fun)
{
    env->setValue(name, new BuildinFunctionType(fun, name));
}
