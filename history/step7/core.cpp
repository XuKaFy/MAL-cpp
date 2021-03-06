#include "core.h"

void Core::registerBasicFunction(Environment* env)
{
    registerFunction(env, "+", FUNCTION(o) {
        Float num = 0;
        FOREACH(m, o, { num += GETFLOAT(m); });
        return new FloatType(num);
    });
    registerFunction(env, "*", FUNCTION(o) {
        Float num = 1;
        FOREACH(m, o, { num *= GETFLOAT(m); });
        return new FloatType(num);
    });
    registerFunction(env, "-", FUNCTION(o) {
        Float num = GETFLOAT(GET(o));
        FOREACH(m, o, { num -= GETFLOAT(m); });
        return new FloatType(num);
    });
    registerFunction(env, "/", FUNCTION(o) {
        Float num = GETFLOAT(GET(o));
        FOREACH(m, o, { num /= GETFLOAT(m); });
        return new FloatType(num);
    });
    registerFunction(env, "car", FUNCTION(o) {
        SINGLE(it, o);
        AbstractType* res = CAR(GETLIST(it));
        if(res == nullptr)
            throw Exception("FUNCTION car: Can't \"car\" an empty list");
        return res;
    });
    registerFunction(env, "cdr", FUNCTION(o) {
        SINGLE(it, o);
        AbstractType* res = CDR(GETLIST(it));
        if(res == nullptr)
            throw Exception("FUNCTION car: Can't \"car\" an empty list");
        return res;
    });
    registerFunction(env, "atom", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_ATOM);
    });
    registerFunction(env, "cons", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return CONS(a1->copy(), a2->copy());
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
        return IF(GETFLOAT(a1) > GETFLOAT(a2));
    });
    registerFunction(env, "<", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETFLOAT(a1) < GETFLOAT(a2));
    });
    registerFunction(env, "<=", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETFLOAT(a1) <= GETFLOAT(a2));
    });
    registerFunction(env, ">=", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETFLOAT(a1) >= GETFLOAT(a2));
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
    registerFunction(env, "join-string", FUNCTION(o) {
        String ans;
        FOREACH(m, o, { ans += GETSTRING(m); });
        return new StringType(ans);
    });
    registerFunction(env, "translate-from-string", FUNCTION(o) {
        SINGLE(a1, o);
        return Reader::read(GETSTRING(a1));
    });
    registerFunction(env, "translate-to-string", FUNCTION(o) {
        SINGLE(a1, o);
        return new StringType(Printer::print(a1));
    });
    registerFunction(env, "read-file", FUNCTION(o) {
        SINGLE(a1, o);
        std::ifstream stm(GETSTRING(a1));
        std::stringstream buffer;
        buffer << stm.rdbuf();
        return new StringType(buffer.str());
    });
    registerFunction(env, "write-file", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        std::ofstream stm(GETSTRING(a1));
        stm << Printer::print(a2);
        return new AbstractType();
    });
}

void Core::registerFunction(Environment *env, String name, Function fun)
{
    env->setValue(name, new BuildinFunctionType(fun, name));
}
