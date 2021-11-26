#include "core.h"

void Core::registerBasicFunction(Pointer<Environment> env)
{
    registerFunction(env, "+", FUNCTION(o) {
        Number num = 0;
        FOREACH(m, o, { num += GETNUMBER(m); });
        return VALUE(Memory::dispatch(num));
    });
    registerFunction(env, "*", FUNCTION(o) {
        Number num = 1;
        FOREACH(m, o, { num *= GETNUMBER(m); });
        return VALUE(Memory::dispatch(num));
    });
    registerFunction(env, "-", FUNCTION(o) {
        Number num = GETNUMBER(GET(o));
        FOREACH(m, o, { num -= GETNUMBER(m); });
        return VALUE(Memory::dispatch(num));
    });
    registerFunction(env, "/", FUNCTION(o) {
        Number num = GETNUMBER(GET(o));
        FOREACH(m, o, { num /= GETNUMBER(m); });
        return VALUE(Memory::dispatch(num));
    });
    registerFunction(env, "car", FUNCTION(o) {
        SINGLE(it, o);
        ValueType res = CAR(GETLIST(it));
        if(!res)
            throw Exception("FUNCTION car: Can't \"car\" an empty list");
        return res;
    });
    registerFunction(env, "cdr", FUNCTION(o) {
        SINGLE(it, o);
        ValueType res = CDR(GETLIST(it));
        if(!res)
            throw Exception("FUNCTION car: Can't \"car\" an empty list");
        return res;
    });
    registerFunction(env, "cons", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return VALUE(CONS(a1->copy(), a2->copy()));
    });
    registerFunction(env, "eq", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(EQ(a1, a2));
    });
    registerFunction(env, "list", FUNCTION(o) {
        return VALUE(o);
    });
    registerFunction(env, "empty?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(ISEMPTY(it));
    });
    registerFunction(env, "list?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_LIST 
                && ISLIST(CONVERT(o, ListType)));
    });
    registerFunction(env, "pair?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_LIST && !ISEMPTY(GETLIST(it)) && !ISEMPTY(CDR(GETLIST(it))));
    });
    registerFunction(env, "atom?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_ATOM);
    });
    registerFunction(env, "string?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_STRING);
    });
    registerFunction(env, "lambda?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_LAMBDA);
    });
    registerFunction(env, "number?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_NUMBER);
    });
    registerFunction(env, "macro?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_MACRO);
    });
    registerFunction(env, "buildin?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_BUILDIN);
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
        return Helper::constantVoid();
    });
    registerFunction(env, "newline", FUNCTION(o) {
        NONEARG(o)
        std::cout << std::endl;
        return Helper::constantVoid();
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
        return VALUE(Memory::dispatch(s));
    });
    registerFunction(env, "print-string", FUNCTION(o) {
        FOREACH(m, o, { std::cout << Printer::printWithEscape(GETSTRING(m)); });
        return Helper::constantVoid();
    });
    registerFunction(env, "join-string", FUNCTION(o) {
        String ans;
        FOREACH(m, o, { ans += GETSTRING(m); });
        return VALUE(Memory::dispatch(ans, true));
    });
    registerFunction(env, "translate-from-string", FUNCTION(o) {
        SINGLE(a1, o);
        return Reader::read(GETSTRING(a1));
    });
    registerFunction(env, "translate-to-string", FUNCTION(o) {
        SINGLE(a1, o);
        return VALUE(Memory::dispatch(Printer::print(a1), true));
    });
    registerFunction(env, "read-file", FUNCTION(o) {
        SINGLE(a1, o);
        std::ifstream stm(GETSTRING(a1));
        if(!stm.is_open())
            throw Exception("Core::read-file: File not existed");
        std::stringstream buffer;
        buffer << stm.rdbuf();
        return VALUE(Memory::dispatch(buffer.str(), true));
    });
    registerFunction(env, "write-file", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        std::ofstream stm(GETSTRING(a1));
        if(!stm.is_open())
            throw Exception("Core::write-file: File not existed");
        stm << Printer::print(a2);
        return Helper::constantVoid();
    });
    registerFunction(env, "time", FUNCTION(o) {
        NONEARG(o)
        return VALUE(Memory::dispatch(Number(time(0))));
    });
}

void Core::registerFunction(Pointer<Environment> env, String name, Function fun)
{
    env->setValue(name, VALUE(Memory::dispatch(fun, name)));
}
