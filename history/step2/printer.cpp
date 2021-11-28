#include "printer.h"

String Printer::print_str(AbstractType *obj)
{
    switch(obj->type()) {
    case Type::TYPE_NULL:
        return "nil";
    case Type::TYPE_ATOM:
        return castAtom(Helper::convert<AtomType*>(obj)->atom());
    case Type::TYPE_NUMBER:
        return castNumber(Helper::convert<NumberType*>(obj)->number());
    case Type::TYPE_LIST:
        return castList((Helper::convert<ListType*>(obj)));
    case Type::TYPE_BUILDIN_FUNCTION:
        return castBuildinFunction(Helper::convert<BuildinFunctionType*>(obj));
    case Type::TYPE_STRING:
        return castString(Helper::convert<StringType*>(obj)->string());
    }
    return "nil";
}

String Printer::castNumber(Number n)
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
    return "##buildinFunction_" + n->name();
}