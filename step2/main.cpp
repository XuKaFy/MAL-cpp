#include <iostream>
#include <string>

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
        std::cout << printer.print_str(obj) << std::endl;
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
                case Exception::EXP_EVAL_CANNOT_EXECUTE:
                    std::cout << "Eval: cannot execute" << std::endl;
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
                }
            }
        }
    }
    void generateMainEnvironment() {
        environment.setValue("+", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                Number num = 0;
                Helper::foreach(o, [&](AbstractType* o) {
                    num += Helper::convert<NumberType*>(o, Type::TYPE_NUMBER)->number();
                });
                return new NumberType(num);
            },
            "plus"
        ));
        environment.setValue("*", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                Number num = 1;
                Helper::foreach(o, [&](AbstractType* o) {
                    num *= Helper::convert<NumberType*>(o, Type::TYPE_NUMBER)->number();
                });
                return new NumberType(num);
            },
            "mult"
        ));
        environment.setValue("-", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                Number num = Helper::convert<NumberType*>(Helper::car(o), Type::TYPE_NUMBER)
                            ->number();
                Helper::next(o);
                Helper::foreach(o, [&](AbstractType* o) {
                    num -= Helper::convert<NumberType*>(o, Type::TYPE_NUMBER)->number();
                });
                return new NumberType(num);
            },
            "sub"
        ));
        environment.setValue("/", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                Number num = Helper::convert<NumberType*>(Helper::car(o), Type::TYPE_NUMBER)
                            ->number();
                Helper::next(o);
                Helper::foreach(o, [&](AbstractType* o) {
                    num /= Helper::convert<NumberType*>(o, Type::TYPE_NUMBER)->number();
                });
                return new NumberType(num);
            },
            "div"
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
                    return new AtomType("t");
                return new ListType();
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
                    return new AtomType("t");
                return new ListType();
            },
            "eq"
        ));
        environment.setValue("list", new BuildinFunctionType(
            [](ListType* o) -> AbstractType* {
                return o;
            },
            "list"
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