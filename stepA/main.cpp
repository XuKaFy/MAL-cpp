#include <iostream>
#include <string>

#include "environment.h"
#include "reader.h"
#include "evaluator.h"
#include "printer.h"
#include "core.h"
#include "common.h"

#define EOF_EXCEPTION Exception("Interface::read: EOF")

class Interface {
public:
    String read() {
        String str;
        if(!getline(std::cin, str))
            throw EOF_EXCEPTION;
        return str;
    }
    AbstractType* eval(String exp) {
        AbstractType* root = Reader::read(exp);
        return evaluator.eval(root, &environment);
    }
    void print(AbstractType* obj) {
        std::cout << Printer::print(obj) << std::endl;
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
        } catch(AbstractType* k) {
            std::cout << "rep: Caught error: " + Printer::print(k) << std::endl;
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
    }
    void generateMainEnvironment() {
        try {
            Core::registerBasicFunction(&environment);
            environment.setValue("true", eval("(quote t)"));
            environment.setValue("false", eval("(quote ())"));
            environment.setValue("not", eval("(lambda (x) (if x false true))"));
            environment.setValue("println", eval("(lambda (x) (print x) (newline))"));
            environment.setValue("load-file", eval("(lambda (x) (eval (translate-from-string (read-file x))))"));
        } catch (Exception e) {
            std::cout << e << std::endl;
        }
    }

private:
    Evaluator evaluator;
    Environment environment;
} interface;

int main()
{
    interface.generateMainEnvironment();
    interface.loop();
    return 0;
}