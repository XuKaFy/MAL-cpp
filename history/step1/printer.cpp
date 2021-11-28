#include "printer.h"

String Printer::print_str(AbstractType *obj)
{
    switch(obj->type()) {
    case Type::TYPE_NULL:
        return "nil";
    case Type::TYPE_ATOM:
        return castAtom(static_cast<AtomType*>(obj)->atom());
    case Type::TYPE_NUMBER:
        return castNumber(static_cast<NumberType*>(obj)->number());
    case Type::TYPE_LIST:
        return castList(obj);
    case Type::TYPE_STRING:
        return "-string-";
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

String Printer::castList(AbstractType *n)
{
    String ans = "(";
    ListType *p;
    while(true) {
        p = static_cast<ListType*>(n);
        if(p->isEmpty())
            break;
        if(p->isLast()) {
            ans += print_str(p->list().first);
            if(p->list().second->type() != Type::TYPE_LIST) 
                ans += " . " 
                    + print_str(p->list().second);
            break;
        }
        ans += print_str(p->list().first) + " ";
        n = p->list().second;
    }
    ans += ")";
    return ans;
}