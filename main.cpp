#include <iostream>
#include <string>

#include "environment.h"
#include "definition.h"
#include "evaluator.h"
#include "printer.h"
#include "reader.h"
#include "type.h"
#include "core.h"

#define EOF_EXCEPTION Exception("Interface::read: EOF")

class Interface {
public:
    Interface() {
        environment = new EnvironmentType();
    }

    String read() {
        String str;
        if(!getline(std::cin, str))
            throw EOF_EXCEPTION;
        return str;
    }

    ValuePointer eval(String exp) {
        ValuePointer root = VALUE(Reader::read(exp));
        //print(root);
        return evaluator.eval(root, environment);
    }

    void print(ValuePointer obj) {
        std::cout << Printer::print(obj, true) << std::endl;
    }

    bool rep() {
        String str;
        std::cout << "user> ";
        try {
            str = read();
        } catch(Exception e) {
            return false;
        }
        try {
            print(eval(str));
        } catch(ValuePointer k) {
            std::cout << "rep: Caught error: " + Printer::print(k, true) << std::endl;
            return false;
        }
        return true;
    }

    void loop() {
        bool flag = true;
        while(flag) {    
            try {
                flag = rep();
            } catch(Exception e) {
                if(e == EOF_EXCEPTION)
                    break;
                std::cout << e << std::endl;
            }
        }
        environment->clear();
    }

    void generateMainEnvironment(int argc, char* argv[]) {
        try {
            Core::registerBasicFunction(environment);
            environment->setValue("*host-language*", eval("\"C++\""));
            environment->setValue("true",   TRUE);
            environment->setValue("false",  FALSE);
            environment->setValue("nil",    VOID);
            environment->setValue("not", eval("(fn* (x) (if x false true))"));
            environment->setValue("load-file", eval("(fn* (f) (eval (read-string (str \"(do \" (slurp f) \")\"))))"));
            environment->setValue("*gensym-counter*", eval("(atom 0)"));
            environment->setValue("gensym", eval("(fn* () (symbol (str \"G__\" (swap! *gensym-counter* (fn* (x) (+ 1 x))))))"));
            eval("(defmacro! or (& xs) (if (empty? xs) '() (if (= 1 (count xs)) (first xs) (let* ((condvar (gensym))) `(let* ((~condvar ~(first xs))) (if ~condvar ~condvar (or ~@(rest xs))))))))");
        } catch (Exception e) {
            std::cout << e << std::endl;
        }
        eval("(println (str \"MAL Interpreter [\" *host-language* \"]\"))");
        if(argc > 1) {
            eval(String("(load-file \"") + argv[1] + "\")");
            Vector v;
            for(int i=2; i<argc; ++i)
                v.push_back(VALUE(Memory::dispatchString(argv[i])));
            environment->setValue("*ARGV*",VALUE(Memory::dispatchVector(v)));
        }
    }

private:
    Evaluator           evaluator;
    EnvironmentPointer  environment;
} interface;

int main(int argc, char* argv[])
{
    interface.generateMainEnvironment(argc, argv);
    interface.loop();
    return 0;
}