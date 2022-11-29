#include "IR.hpp"
#include "iostream"
#include "vector"
#include "tuple"
#include "AST.hpp"
IRNode::IRNode() {
}
std::string IRNode::ToString()  const {
    return "IRNode";
}
IRProgram::IRProgram(std::vector<IRNode*> declList) {
    this->decls = declList;
}
std::string IRProgram::ToString()  const {
    std::string str = "";
    int i = 0;
    while ((i) < this->decls.size()) {
        str += ((this->decls[i])->ToString());
        if ((i) < ((this->decls.size()) - 1)) {
            str += "\n";
        }
        i++;
    }
    return str;
}
auto IRBody::IRProgram() {
}
std::string IRBody::ToString()  const {
    std::string str = "{\n";
    int i = 0;
    while ((i) < this->decls.size()) {
        str += ((this->decls[i])->ToString()) + "\n";
        i++;
    }
    str += "}";
    return str;
}
IRExpr::IRExpr(std::string expr) {
    this->expr = expr;
}
std::string IRExpr::ToString()  const {
    return this->expr;
}
std::string IRFunction::ToString()  const {
    std::string str = "";
    if ((this->attr & Attribute::Static) != 0) {
        str += "static ";
    }
    if ((this->attr & Attribute::Virtual) != 0) {
        str += "virtual ";
    }
    str += this->returnType + " " + this->name + "(";
    int i = 0;
    while ((i) < this->params.size()) {
        str += (std::get<0>(this->params[i])) + " " + (std::get<1>(this->params[i]));
        if ((i) < ((this->params.size()) - 1)) {
            str += ", ";
        }
        i++;
    }
    str += ")";
    if ((this->attr & Attribute::Const) != 0) {
        str += " const";
    }
    str += " ";
    str += this->body->ToString();
    return str;
}
std::string IRReturn::ToString()  const {
    return "return " + (this->expr->ToString()) + ";";
}
std::string IRIf::ToString()  const {
    std::string str = "if (" + (this->cond->ToString()) + ") " + (this->body->ToString());
    int i = 0;
    while ((i) < this->elifs.size()) {
        str += " else " + ((this->elifs[i])->ToString());
        i++;
    }
    if (this->elseBody != 0) {
        str += " else " + (this->elseBody->ToString());
    }
    return str;
}
std::string IRVariable::ToString()  const {
    std::string str = "";
    str += this->vType + " ";
    if ((this->attr & Attribute::Const) != 0) {
        str += "const ";
    }
    str += this->name;
    if (this->value != 0) {
        str += " = " + (this->value->ToString());
    }
    str += ";";
    return str;
}
