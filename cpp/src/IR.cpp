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
std::string IRNode::GetFullName()  const {
    return "IRNode";
}
bool IRNode::IsTemplate()  const {
    return false;
}
std::string IRNode::GetHeader()  const {
    return "";
}
std::string IRNode::GetSource()  const {
    return "";
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
        str += ((this->decls[i])->ToString()) + ";\n";
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
std::string IRFunction::GetFullName()  const {
    if ((this->parent) != nullptr) {
        return (this->parent->GetFullName()) + "::" + this->name;
    }
    return this->name;
}
bool IRFunction::IsTemplate()  const {
    return (this->templateParams.size()) > 0 || (this->parent) != nullptr && (this->parent->IsTemplate());
}
std::string IRFunction::GetHeader()  const {
    return this->GetFullName();
}
std::string IRFunction::GetSource()  const {
    return this->GetFullName();
}
std::string IRReturn::ToString()  const {
    return "return " + (this->expr->ToString());
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
std::string IRWhile::ToString()  const {
    return "while (" + (this->cond->ToString()) + ") " + (this->body->ToString());
}
std::string IRBreak::ToString()  const {
    return "break";
}
std::string IRContinue::ToString()  const {
    return "continue";
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
    return str;
}
std::string IRImport::ToString()  const {
    return "import \"" + this->path + '\"';
}
std::string IRClass::ToString()  const {
    std::string str = "";
    return str;
}
