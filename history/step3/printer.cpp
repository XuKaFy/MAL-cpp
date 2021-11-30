#include "printer.h"

String Printer::print_str(AbstractType *obj)
{
    switch(obj->type()) {
    case Type::TYPE_NULL:
        return "#<nil>";
    case Type::TYPE_ATOM:
        return castAtom(Helper::convert<AtomType*>(obj)->atom());
    case Type::TYPE_FLOAT:
        return castFloat(Helper::convert<FloatType*>(obj)->number());
    case Type::TYPE_LIST:
        return castList((Helper::convert<ListType*>(obj)));
    case Type::TYPE_BUILDIN_FUNCTION:
        return castBuildinFunction(Helper::convert<BuildinFunctionType*>(obj));
    case Type::TYPE_STRING:
        return castString(Helper::convert<StringType*>(obj)->string());
    case Type::TYPE_LAMBDA:
        return castLambda(Helper::convert<LambdaType*>(obj));
    }
    return "#<nil>";
}

String Printer::castFloat(Float n)
{
    std::ostringstream s;
    s << n;
    return s.str();
}

String Printer::castAtom(Atom n)
{
    return n;
}

String Printer::castList(ListType *n)
{
    String ans = "(";
    if(!Helper::isEmpty(n)) {
        bool first = true;
        AbstractType* remain = Helper::foreach(n, [&](AbstractType* o) {
            if(first) {
                first = false;
            } else {
                ans += " ";
            }
            ans += print_str(o);
        });
        if(!Helper::isEmpty(remain))
            ans += " . " + print_str(remain);
    }
    ans += ")";
    return ans;
}

String Printer::castString(String n)
{
    return '"' + n + '"';
}

String Printer::castBuildinFunction(BuildinFunctionType* n)
{
    return "#<procedure:" + n->name() + ">";
}

String Printer::castLambda(LambdaType* n)
{
    String ans = "#<lambda:(";
    ans += print_str(n->arg());
    ans += " " + print_str(n->body());
    ans += ")>";
    return ans;
}