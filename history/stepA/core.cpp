#include "core.h"

void Core::registerBasicFunction(EnvironmentPointer env)
{
    registerFunction(env, "+", FUNCTION(o) {
        Float num = 0;
        FOREACH(m, o, { num += GETFLOAT(m); });
        return VALUE(Memory::dispatchFloat(num));
    });

    registerFunction(env, "*", FUNCTION(o) {
        Float num = 1;
        FOREACH(m, o, { num *= GETFLOAT(m); });
        return VALUE(Memory::dispatchFloat(num));
    });

    registerFunction(env, "-", FUNCTION(o) {
        Float num = GETFLOAT(GET(o));
        FOREACH(m, o, { num -= GETFLOAT(m); });
        return VALUE(Memory::dispatchFloat(num));
    });

    registerFunction(env, "/", FUNCTION(o) {
        Float num = GETFLOAT(GET(o));
        FOREACH(m, o, { num /= GETFLOAT(m); });
        return VALUE(Memory::dispatchFloat(num));
    });

    registerFunction(env, "car", FUNCTION(o) {
        SINGLE(it, o);
        ValuePointer res = CAR(GETLIST(it));
        if(!res)
            throw Exception("FUNCTION car: Can't \"car\" an empty list");
        return res;
    });

    registerFunction(env, "cdr", FUNCTION(o) {
        SINGLE(it, o);
        ValuePointer res = CDR(GETLIST(it));
        if(!res)
            throw Exception("FUNCTION car: Can't \"car\" an empty list");
        return res;
    });

    registerFunction(env, "cons", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return VALUE(CONS(a1->copy(), a2->copy()));
    });

    registerFunction(env, SYM_EQ, FUNCTION(o) {
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
        return IF(it->type() == Type::TYPE_LIST && 
                  !ISEMPTY(GETLIST(it)) && 
                  !ISEMPTY(CDR(GETLIST(it))));
    });

    registerFunction(env, "symbol?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_SYMBOL);
    });

    registerFunction(env, "string?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_STRING);
    });

    registerFunction(env, "fn?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_LAMBDA);
    });

    registerFunction(env, "number?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_FLOAT);
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
        return VOID;
    });

    registerFunction(env, "newline", FUNCTION(o) {
        NONEARG(o)
        std::cout << std::endl;
        return VOID;
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
        return VALUE(Memory::dispatchString(s));
    });

    registerFunction(env, "print-string", FUNCTION(o) {
        FOREACH(m, o, { std::cout << Printer::printWithEscape(GETSTRING(m)); });
        return VOID;
    });

    registerFunction(env, "join-string", FUNCTION(o) {
        String ans;
        FOREACH(m, o, { ans += GETSTRING(m); });
        return VALUE(Memory::dispatchString(ans));
    });

    registerFunction(env, "translate-from-string", FUNCTION(o) {
        SINGLE(a1, o);
        return Reader::read(GETSTRING(a1));
    });

    registerFunction(env, "translate-to-string", FUNCTION(o) {
        SINGLE(a1, o);
        return VALUE(Memory::dispatchString(Printer::print(a1)));
    });

    registerFunction(env, "read-file", FUNCTION(o) {
        SINGLE(a1, o);
        std::ifstream stm(GETSTRING(a1));
        if(!stm.is_open())
            throw Exception("Core::read-file: File not existed");
        std::stringstream buffer;
        buffer << stm.rdbuf();
        return VALUE(Memory::dispatchString(buffer.str()));
    });

    registerFunction(env, "write-file", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        std::ofstream stm(GETSTRING(a1));
        if(!stm.is_open())
            throw Exception("Core::write-file: File not existed");
        stm << Printer::print(a2);
        return VOID;
    });

    registerFunction(env, "time", FUNCTION(o) {
        NONEARG(o)
        return VALUE(Memory::dispatchFloat(Float(time(0))));
    });
}

void Core::registerFunction(EnvironmentPointer env, String name, Function fun)
{
    env->setValue(name, VALUE(Memory::dispatchBuildin(fun, name)));
}
