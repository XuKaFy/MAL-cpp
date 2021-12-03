#include "core.h"

void Core::registerBasicFunction(EnvironmentPointer env)
{
#define REGISTER_INTEGER_FUN(sgn, name) \
    registerFunction(env, name, FUNCTION(o) { \
        Integer num = GETINTEGER(GET(o)); \
        FOREACH(m, o, { num sgn##= GETINTEGER(m); }); \
        return VALUE(Memory::dispatchInteger(num)); \
    });

#define REGISTER_NUMBER_FUN(sgn) \
    registerFunction(env, #sgn, FUNCTION(o) { \
        Type ansType = CAR(o)->type(); \
        Integer ans1 = 0; \
        Float  ans2 = 0; \
        switch(CAR(o)->type()) { \
        case Type::TYPE_INTEGER: \
            ans1 = GETINTEGER(GET(o)); \
            break; \
        case Type::TYPE_FLOAT: \
            ans2 = GETFLOAT(GET(o)); \
            break; \
        default: \
            throw Exception("Core::FLOAT_FUN: Not a number"); \
        } \
        FOREACH(m, o, { \
            switch(m->type()) { \
            case Type::TYPE_INTEGER: \
                if(ansType == Type::TYPE_INTEGER) \
                    ans1 sgn##= GETINTEGER(m); \
                else \
                    ans2 sgn##= GETINTEGER(m); \
                break; \
            case Type::TYPE_FLOAT: \
                if(ansType == Type::TYPE_INTEGER && m->type() == Type::TYPE_FLOAT) { \
                    ans2 = ans1; \
                    ansType = Type::TYPE_FLOAT; \
                } \
                if(ansType == Type::TYPE_INTEGER) \
                    ans1 sgn##= GETFLOAT(m); \
                else \
                    ans2 sgn##= GETFLOAT(m); \
                break; \
            default: \
                throw Exception("Core::FLOAT_FUN: Not a number"); \
            } \
        }); \
        if(ansType == Type::TYPE_INTEGER) \
            return VALUE(Memory::dispatchInteger(ans1)); \
        return VALUE(Memory::dispatchFloat(ans2)); \
    });

    REGISTER_NUMBER_FUN(+)
    REGISTER_NUMBER_FUN(-)
    REGISTER_NUMBER_FUN(*)
    REGISTER_NUMBER_FUN(/)

    REGISTER_INTEGER_FUN(^, "bit-xor")
    REGISTER_INTEGER_FUN(|, "bit-or")
    REGISTER_INTEGER_FUN(&, "bit-and")
    REGISTER_INTEGER_FUN(%, "bit-mod")

    registerFunction(env, "car", FUNCTION(o) {
        SINGLE(it, o);
        ValuePointer res = CAR(GETLIST(it));
        if(!res)
            throw Exception("Core::car: Can't \"car\" an empty list");
        return res;
    });

    registerFunction(env, "cdr", FUNCTION(o) {
        SINGLE(it, o);
        ValuePointer res = CDR(GETLIST(it));
        if(!res)
            throw Exception("Core::car: Can't \"car\" an empty list");
        return res;
    });

    registerFunction(env, "first", FUNCTION(o) {
        SINGLE(it, o);
        if(it->type() == Type::TYPE_LIST) {
            ValuePointer res = CAR(GETLIST(it));
            if(!res)
                throw Exception("Core::first: Can't \"first\" an empty list");
            return res;
        } else if(it->type() == Type::TYPE_VECTOR) {
            const Vector &v = GETVECTOR(it);
            if(v.empty())
                throw Exception("Core::first: Can't \"first\" an empty vector");    
            return *v.begin();
        }
        throw Exception("Core::first: Can't \"first\" this type"); 
    });

    registerFunction(env, "rest", FUNCTION(o) {
        SINGLE(it, o);
        if(it->type() == Type::TYPE_LIST) {
            ValuePointer res = CDR(GETLIST(it));
            if(!res)
                throw Exception("Core::rest: Can't \"rest\" an empty list");
            return res;
        } else if(it->type() == Type::TYPE_VECTOR) {
            const Vector &v = GETVECTOR(it);
            if(v.empty())
                throw Exception("Core::rest: Can't \"rest\" an empty vector");    
            return VALUE(Memory::dispatchVector(Vector(v.begin() + 1, v.end())));
        }
        throw Exception("Core::rest: Can't \"rest\" this type"); 
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
        return VALUE(o->copy());
    });

    registerFunction(env, "vector", FUNCTION(o) {
        Vector v;
        FOREACH(m, o, {
            v.push_back(m->copy());
        });
        return VALUE(Memory::dispatchVector(v));
    });

    registerFunction(env, "hash-map", FUNCTION(o) {
        Map p;
        Keyword key;
        FOREACH(m, o, {
            if(key.empty()) {
                key = GETKEYWORD(m);
            } else {
                p[key] = m->copy();
                key.clear();
            }
        });
        if(!key.empty())
            throw Exception("Core::hashmap: not match");
        return VALUE(Memory::dispatchMap(p));
    });

    registerFunction(env, "get", FUNCTION(o) {
        const Map& map = GETMAP(GET(o));
        const Keyword& key = GETKEYWORD(GET(o));
        if(ISEMPTY(o)) {
            if(map.count(key) == 0) 
                throw Exception("Core::get: No elem");
            return map.at(key);
        }
        SINGLE(def, o);
        if(map.count(key) == 0) 
            return def;
        return map.at(key);
    });

    registerFunction(env, "assoc", FUNCTION(o) {
        Map map = GETMAP(GET(o)->copy());
        Keyword key;
        FOREACH(m, o, {
            if(key.empty()) {
                key = GETKEYWORD(m);
            } else {
                map[key] = m->copy();
                key.clear();
            }
        });
        if(!key.empty())
            throw Exception("Core::assoc: Not match");
        return VALUE(Memory::dispatchMap(map));
    });

    registerFunction(env, "dissoc", FUNCTION(o) {
        Map map = GETMAP(GET(o)->copy());
        FOREACH(m, o, {
            map.erase(GETKEYWORD(m));
        });
        return VALUE(Memory::dispatchMap(map));
    });

#define SYMBOL_CONVERT(name, retType) \
    registerFunction(env, name, FUNCTION(o) { \
        SINGLE(it, o) \
        String ans; \
        switch(it->type()) { \
        case Type::TYPE_SYMBOL: \
        case Type::TYPE_KEYWORD: \
        case Type::TYPE_STRING: \
            ans = GETSYMBOL(it); \
            break; \
        default: \
            throw Exception("Core::SYMBOL_CONVERT: Not a string type"); \
        } \
        return VALUE(Memory::dispatch##retType(ans)); \
    });

    SYMBOL_CONVERT("keyword", Keyword)
    SYMBOL_CONVERT("symbol",  Symbol)
    SYMBOL_CONVERT("string",  String)

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

    registerFunction(env, "map?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_HASHMAP);
    });

    registerFunction(env, "vector?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_VECTOR);
    });

    registerFunction(env, "keyword?", FUNCTION(o) {
        SINGLE(it, o);
        return IF(it->type() == Type::TYPE_KEYWORD);
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

    registerFunction(env, "nil?", FUNCTION(o) {
        SINGLE(a1, o);
        return IF(a1->type() == Type::TYPE_NULL);
    });

#define GETNUM(x) (x->type() == Type::TYPE_INTEGER? GETINTEGER(x): \
                  (x->type() == Type::TYPE_FLOAT?  GETFLOAT(x) : \
                   throw Exception("Core::GETNUM: Not a number")))

    registerFunction(env, ">", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETNUM(a1) > GETNUM(a2));
    });

    registerFunction(env, "<", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETNUM(a1) < GETNUM(a2));
    });

    registerFunction(env, "<=", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETNUM(a1) <= GETNUM(a2));
    });

    registerFunction(env, ">=", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETNUM(a1) >= GETNUM(a2));
    });

    registerFunction(env, "newline", FUNCTION(o) {
        NONEARG(o)
        std::cout << std::endl;
        return VOID;
    });

    registerFunction(env, "readline", FUNCTION(o) {
        NONEARG(o)
        String s;
        getline(std::cin, s);
        return Reader::read(s);
    });

    registerFunction(env, "print-str", FUNCTION(o) {
        bool first = true;
        std::stringstream ss;
        FOREACH(m, o, { 
            if(first) {
                first = false;
            } else {
                ss << " ";
            }
            ss << Printer::print(m, false);
        });
        return VALUE(Memory::dispatchString(ss.str()));
    });

    registerFunction(env, "pr-str", FUNCTION(o) {
        bool first = true;
        std::stringstream ss;
        FOREACH(m, o, {
            if(first) {
                first = false;
            } else {
                ss << " ";
            }
            ss << Printer::print(m, true);
        });
        return VALUE(Memory::dispatchString(ss.str()));
    });

    registerFunction(env, "print", FUNCTION(o) {
        bool first = true;
        FOREACH(m, o, { 
            if(first) {
                first = false;
            } else {
                std::cout << " ";
            }
            std::cout << Printer::print(m, false);
        });
        return VOID;
    });

    registerFunction(env, "pr", FUNCTION(o) {
        bool first = true;
        FOREACH(m, o, {
            if(first) {
                first = false;
            } else {
                std::cout << " ";
            }
            std::cout << Printer::print(m, true);
        });
        return VOID;
    });

    registerFunction(env, "println", FUNCTION(o) {
        bool first = true;
        FOREACH(m, o, { 
            if(first) {
                first = false;
            } else {
                std::cout << " ";
            }
            std::cout << Printer::print(m, false);
        });
        std::cout << std::endl;
        return VOID;
    });

    registerFunction(env, "prn", FUNCTION(o) {
        bool first = true;
        FOREACH(m, o, {
            if(first) {
                first = false;
            } else {
                std::cout << " ";
            }
            std::cout << Printer::print(m, true);
        });
        std::cout << std::endl;
        return VOID;
    });

    registerFunction(env, "str", FUNCTION(o) {
        String s;
        FOREACH(m, o, {
            if(m->type() == Type::TYPE_STRING)
                s += GETSTRING(m);
            else
                s += Printer::print(m, false);
        });
        return VALUE(Memory::dispatchString(s));
    });

    registerFunction(env, "read-string", FUNCTION(o) {
        SINGLE(a1, o);
        return Reader::read(GETSTRING(a1));
    });

    registerFunction(env, "slurp", FUNCTION(o) {
        SINGLE(a1, o);
        std::ifstream stm(GETSTRING(a1));
        if(!stm.is_open())
            throw Exception("Core::slurp: File not existed");
        std::stringstream buffer;
        buffer << stm.rdbuf();
        return VALUE(Memory::dispatchString(buffer.str()));
    });

    registerFunction(env, "spit", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        std::ofstream stm(GETSTRING(a1));
        if(!stm.is_open())
            throw Exception("Core::spit: File not existed");
        stm << GETSTRING(a2);
        return VOID;
    });

    registerFunction(env, "nth", FUNCTION(o) {
        DOUBLE(a1, a2, o)
        if(GETINTEGER(a2) < 0)
            throw Exception("Core::nth: pos less than 0");
        switch(a1->type()) {
        case Type::TYPE_VECTOR:
            if((size_t)GETINTEGER(a2) >= GETVECTOR(a1).size())
                throw Exception("Core::nth: pos more than max");
            return GETVECTOR(a1)[GETINTEGER(a2)];
        case Type::TYPE_LIST:
            return Helper::nth(GETLIST(a1), GETINTEGER(a2));
        default:
            throw Exception("Core::nth: not a sequence");
        }
        return VOID;
    });

    registerFunction(env, "count", FUNCTION(o) {
        SINGLE(a1, o)
        switch(a1->type()) {
        case Type::TYPE_LIST:
            return VALUE(Memory::dispatchInteger(Helper::count(GETLIST(a1))));
        case Type::TYPE_VECTOR:
            return VALUE(Memory::dispatchInteger(GETVECTOR(a1).size()));
        case Type::TYPE_HASHMAP:
            return VALUE(Memory::dispatchInteger(GETMAP(a1).size()));
        default:
            throw Exception("Core::count: Not countable");
        }
        return VOID;
    });

    registerFunction(env, "concat", FUNCTION(o) {
        ListPointer root = Helper::toList(GET(o));
        ListPointer current = root;
        FOREACH(m, o, {
            while(!Helper::isLast(current))
                NEXT(current);
            current->setSecond(VALUE(Helper::toList(m)));
        });
        return VALUE(root);
    });

    registerFunction(env, "conj", FUNCTION(o) {
        ValuePointer v = GET(o);
        switch(v->type()) {
        case Type::TYPE_LIST: {
                ListPointer l = GETLIST(v->copy());
                FOREACH(m, o, {
                    l = Memory::dispatchList(m, VALUE(l));
                });
                v = VALUE(l);
            }
            break;
        case Type::TYPE_VECTOR: {
                Vector vec = GETVECTOR(v);
                FOREACH(m, o, {
                    vec.push_back(m->copy());
                });
                v = VALUE(Memory::dispatchVector(vec));
            }
            break;
        case Type::TYPE_HASHMAP: {
                Map map = GETMAP(v);
                Keyword key;
                FOREACH(m, o, {
                    if(key.empty()) {
                        key = GETKEYWORD(m);
                    } else {
                        map[key] = m->copy();
                        key.clear();
                    }
                });
                if(!key.empty())
                    throw Exception("Core::conj: Key-Value cannot match");
                v = VALUE(Memory::dispatchMap(map));
            }
            break;
        default:
            throw Exception("Core::conj: Cannot conj into this type");
        }
        return v;    
    });

    registerFunction(env, "seq", FUNCTION(o) {
        SINGLE(a1, o)
        const Map& map = GETMAP(a1);
        ListPointer root = Memory::dispatchList();
        ListPointer current = root;
        for(auto i : map) {
            Vector v;
            v.push_back(VALUE(Memory::dispatchKeyword(i.first)));
            v.push_back(i.second->copy());
            current = Helper::append(current, VALUE(Memory::dispatchVector(v)));
        }
        current->setSecond(FALSE);
        return VALUE(root);
    });

    registerFunction(env, "sequential?", FUNCTION(o) {
        SINGLE(a1, o)
        return IF(a1->type() == Type::TYPE_LIST || a1->type() == Type::TYPE_VECTOR);
    });

    registerFunction(env, "contains?", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        return IF(GETMAP(a1).count(GETKEYWORD(a2)));
    });

    registerFunction(env, "keys", FUNCTION(o) {
        SINGLE(a1, o);    
        ListPointer root = Memory::dispatchList();
        ListPointer current = root;
        const Map& map = GETMAP(a1);
        for(auto i : map)
            current = Helper::append(current, VALUE(Memory::dispatchKeyword(i.first)));
        if(CAR(current))
            current->setSecond(FALSE);
        return VALUE(root);
    });

    registerFunction(env, "values", FUNCTION(o) {
        SINGLE(a1, o);    
        ListPointer root = Memory::dispatchList();
        ListPointer current = root;
        const Map& map = GETMAP(a1);
        for(auto i : map)
            current = Helper::append(current, i.second->copy());
        if(CAR(current))
            current->setSecond(FALSE);
        return VALUE(root);
    });

    registerFunction(env, "with-meta", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        ValuePointer ans = a1->copy();
        ans->setMeta(a2);
        return ans;
    });

    registerFunction(env, "meta", FUNCTION(o) {
        SINGLE(a1, o);
        if(a1->meta().empty())
            throw Exception("Core::meta: No meta");
        return a1->meta();
    });

    registerFunction(env, "atom", FUNCTION(o) {
        SINGLE(a1, o);
        return VALUE(Memory::dispatchAtom(a1));
    });

    registerFunction(env, "atom?", FUNCTION(o) {
        SINGLE(a1, o);
        return IF(a1->type() == Type::TYPE_ATOM);
    });

    registerFunction(env, "deref", FUNCTION(o) {
        SINGLE(a1, o);
        return GETATOM(a1)->reference();
    });

    registerFunction(env, "reset!", FUNCTION(o) {
        DOUBLE(a1, a2, o);
        GETATOM(a1)->setReference(a2);
        return VOID;
    });
}

void Core::registerFunction(EnvironmentPointer env, String name, Function fun)
{
    env->setValue(name, VALUE(Memory::dispatchBuildin(fun, name)));
}
