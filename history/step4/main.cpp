#include <iostream>
#include <string>

#include "environment.h"
#include "reader.h"
#include "evaluator.h"
#include "printer.h"
#include "common.h"

class Interface {
public:
    String read() {
        String str;
        if(!getline(std::cin, str))
            throw Exception::EXP_READ_END;
        return str;
    }
    AbstractType* eval(String exp) {
        AbstractType* root = reader.read_str(exp);
        return evaluator.eval(root, &environment);
        //return root;
    }
    void print(AbstractType* obj) {
        std::cout << printer.print(obj) << std::endl;
    }
    bool rep() {
        String str;
        std::cout << "user> ";
        try {
            str = read();
        } catch(Exception e) {
            return false;
        }
        //std::cout << "--- READ END ---" << std::endl;
        print(eval(str));
        return true;
    }
    void loop() {
        bool flag = true;
        while(flag) {    
            try {
                flag = rep();
            } catch(Exception e) {
                switch(e) {
                case Exception::EXP_ANA_MATCHLESS:
                    std::cout << "Analyzer: matchless" << std::endl;
                    break;
                case Exception::EXP_ANA_NONE:
                    std::cout << "Analyzer: none" << std::endl;
                    break;
                case Exception::EXP_ANA_ESCAPE_ERROR:
                    std::cout << "Analyzer: escape error" << std::endl;
                    break;
                case Exception::EXP_READ_END:
                    std::cout << "Reader: reach the end" << std::endl;
                    break;
                case Exception::EXP_READ_MATCH_ERROR:
                    std::cout << "Reader: match error" << std::endl;
                    break;
                case Exception::EXP_READ_MATCH_MORE:
                    std::cout << "Reader: match more than max length" << std::endl;
                    break;
                case Exception::EXP_BUILDIN_FUNCTION_ARG_TYPE_ERROR:
                    std::cout << "Buildin Function: arg type error" << std::endl;
                    break;
                case Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR:
                    std::cout << "Buildin Function: args length error" << std::endl;
                    break;
                case Exception::EXP_ENV_NOT_FOUND:
                    std::cout << "Environment: variable isn't found" << std::endl;
                    break;
                case Exception::EXP_HELPER_CONVERT_NONE:
                    std::cout << "Helper: cannot convert null to other type" << std::endl;
                    break;
                case Exception::EXP_HELPER_GET_ERROR:
                    std::cout << "Helper: get null" << std::endl;
                    break;
                case Exception::EXP_HELPER_APPEND_ERROR:
                    std::cout << "Helper: append a non-list-end" << std::endl;
                    break;
                case Exception::EXP_EVAL_CANNOT_EXECUTE:
                    std::cout << "Eval: cannot execute" << std::endl;
                    break;
                case Exception::EXP_EVAL_BUILDIN_LENGTH_ERROR:
                    std::cout << "Eval: buildin function args length error" << std::endl;
                    break;
                case Exception::EXP_EVAL_BUILDIN_LIST_ERROR:
                    std::cout << "Eval: buildin function takes a fake list" << std::endl;
                    break;
                case Exception::EXP_EVAL_BUILDIN_ARG_ERROR:
                    std::cout << "Eval: buildin function takes a non-flat arg list" << std::endl;
                    break;
                }
            }
        }
    }
    void generateMainEnvironment() {
        environment.setValue("+", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                Float num = 0;
                Helper::foreach(o, [&](AbstractType* o) {
                    num += Helper::convert<FloatType*>(o, Type::TYPE_FLOAT)->number();
                });
                return new FloatType(num);
            },
            "+"
        ));
        environment.setValue("*", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                Float num = 1;
                Helper::foreach(o, [&](AbstractType* o) {
                    num *= Helper::convert<FloatType*>(o, Type::TYPE_FLOAT)->number();
                });
                return new FloatType(num);
            },
            "*"
        ));
        environment.setValue("-", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                Float num = Helper::convert<FloatType*>(Helper::car(o), Type::TYPE_FLOAT)
                            ->number();
                Helper::next(o);
                Helper::foreach(o, [&](AbstractType* o) {
                    num -= Helper::convert<FloatType*>(o, Type::TYPE_FLOAT)->number();
                });
                return new FloatType(num);
            },
            "-"
        ));
        environment.setValue("/", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                Float num = Helper::convert<FloatType*>(Helper::car(o), Type::TYPE_FLOAT)
                            ->number();
                Helper::next(o);
                Helper::foreach(o, [&](AbstractType* o) {
                    num /= Helper::convert<FloatType*>(o, Type::TYPE_FLOAT)->number();
                });
                return new FloatType(num);
            },
            "/"
        ));
        environment.setValue("car", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                AbstractType* ans = Helper::car(Helper::convert<ListType*>(Helper::car(o), Type::TYPE_LIST));
                if(ans == nullptr)
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                return ans;
            },
            "car"
        ));
        environment.setValue("cdr", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                AbstractType* ans = Helper::cdr(Helper::convert<ListType*>(Helper::car(o), Type::TYPE_LIST));
                if(ans == nullptr)
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                return ans;
            },
            "cdr"
        ));
        environment.setValue("atom", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                if(Helper::atom(Helper::car(o)))
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            "atom"
        ));
        environment.setValue("cons", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                AbstractType* a1 = Helper::get(o);
                AbstractType* a2 = Helper::get(o);
                if(!Helper::isEmpty(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                return Helper::cons(a1, a2);
            },
            "cons"
        ));
        environment.setValue("eq", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                AbstractType* a1 = Helper::get(o);
                AbstractType* a2 = Helper::get(o);
                if(!Helper::isEmpty(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                if(Helper::eq(a1, a2))
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            "eq"
        ));
        environment.setValue("list", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                return o;
            },
            "list"
        ));
        environment.setValue("empty?", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                if(Helper::isEmpty(Helper::car(o)))
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            "empty?"
        ));
        environment.setValue("list?", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                AbstractType* arg = Helper::car(o);
                if(arg->type() != Type::TYPE_LIST)
                    return Helper::constantFalse();
                if(Helper::isList(Helper::convert<ListType*>(o)))
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            "list?"
        ));
        environment.setValue("true?", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                if(Helper::isTrue(Helper::car(o)))
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            "true?"
        ));
        environment.setValue("false?", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                if(Helper::isFalse(Helper::car(o)))
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            "false?"
        ));
        environment.setValue(">", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                AbstractType* a1 = Helper::get(o);
                AbstractType* a2 = Helper::get(o);
                if(!Helper::isEmpty(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                Float b1 = Helper::convert<FloatType*>(a1, Type::TYPE_FLOAT)->number();
                Float b2 = Helper::convert<FloatType*>(a2, Type::TYPE_FLOAT)->number();
                if(b1 > b2)
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            ">"
        ));
        environment.setValue("<", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                AbstractType* a1 = Helper::get(o);
                AbstractType* a2 = Helper::get(o);
                if(!Helper::isEmpty(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                Float b1 = Helper::convert<FloatType*>(a1, Type::TYPE_FLOAT)->number();
                Float b2 = Helper::convert<FloatType*>(a2, Type::TYPE_FLOAT)->number();
                if(b1 < b2)
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            "<"
        ));
        environment.setValue(">=", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                AbstractType* a1 = Helper::get(o);
                AbstractType* a2 = Helper::get(o);
                if(!Helper::isEmpty(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                Float b1 = Helper::convert<FloatType*>(a1, Type::TYPE_FLOAT)->number();
                Float b2 = Helper::convert<FloatType*>(a2, Type::TYPE_FLOAT)->number();
                if(b1 >= b2)
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            ">="
        ));
        environment.setValue("<=", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                AbstractType* a1 = Helper::get(o);
                AbstractType* a2 = Helper::get(o);
                if(!Helper::isEmpty(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                Float b1 = Helper::convert<FloatType*>(a1, Type::TYPE_FLOAT)->number();
                Float b2 = Helper::convert<FloatType*>(a2, Type::TYPE_FLOAT)->number();
                if(b1 <= b2)
                    return Helper::constantTrue();
                return Helper::constantFalse();
            },
            "<="
        ));
        environment.setValue("print", new BuildinFunctionType(
            [&](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                if(Helper::car(o)->type() == Type::TYPE_STRING)
                    std::cout << printer.printWithEscape(
                        Helper::convert<StringType*>(Helper::car(o)));    
                else
                    std::cout << printer.print(Helper::car(o));
                return new AbstractType();
            },
            "print"
        ));
        environment.setValue("println", new BuildinFunctionType(
            [&](ListType* o) -> AbstractType* {
                if(Helper::isEmpty(o) || !Helper::isSingle(o))
                    throw Exception::EXP_BUILDIN_FUNCTION_LENGTH_ERROR;
                if(Helper::car(o)->type() == Type::TYPE_STRING)
                    std::cout << printer.printWithEscape(
                        Helper::convert<StringType*>(Helper::car(o))) << std::endl;    
                else
                    std::cout << printer.print(Helper::car(o)) << std::endl;
                return new AbstractType();
            },
            "println"
        ));
    }

private:
    Reader reader;
    Evaluator evaluator;
    Printer printer;
    Environment environment;
} interface;

int main()
{
    interface.generateMainEnvironment();
    interface.loop();
    return 0;
}