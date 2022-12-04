#include "IR.hpp"
#include "iostream"
#include "vector"
#include "string"
#include "tuple"
#include "AST.hpp"
IRNode::IRNode() {
}
std::string IRNode::ToString()  const {
    return "IRNode";
}
std::string IRNode::GetName()  const {
    return "";
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
std::string IRFunction::GetTemplate()  const {
    std::string str = "";
    if ((this->templateParams.size()) > 0) {
        str += "template<";
        int i = 0;
        while ((i) < this->templateParams.size()) {
            std::string tType = std::get<0>(this->templateParams[i]);
            if ((tType) == "type") {
                tType = "typename";
            }
            str += tType + " " + std::get<1>(this->templateParams[i]);
            if ((i) < ((this->templateParams.size()) - 1)) {
                str += ", ";
            }
            i++;
        }
        str += "> ";
    }
    return str;
}
std::string IRFunction::GetParamenters()  const {
    std::string str = "(";
    int i = 0;
    while ((i) < this->params.size()) {
        str += std::get<0>(this->params[i]) + " " + std::get<1>(this->params[i]);
        if ((i) < ((this->params.size()) - 1)) {
            str += ", ";
        }
        i++;
    }
    return str + ")";
}
std::string IRFunction::GetHeader()  const {
    std::string name = this->name;
    std::string protection = "protected: ";
    if ((this->attr) & (Attribute::Public)) {
        protection = "public: ";
    }
    if ((this->attr) & (Attribute::Private)) {
        protection = "private: ";
    }
    std::string retType = this->returnType + " ";
    if (this->parent != 0 && (this->parent->GetName()) == this->name) {
        retType = "";
        if ((this->attr) & (Attribute::Destructor)) {
            name = "~" + this->name;
        }
    }
    std::string declaration = retType + name + (this->GetParamenters());
    std::string const tmp = this->GetTemplate();
    std::string sbody = ";";
    if ((this->IsTemplate())) {
        if ((this->body) != 0) {
            sbody = " " + (this->body->ToString()) + ";";
        }
    }
    if ((this->body) == 0 && (this->attr) & (Attribute::Virtual)) {
        sbody = " = 0;";
    }
    std::string preAttrs = "";
    if (this->returnType != "void" && this->returnType != "auto") {
        preAttrs += "[[nodiscard]] ";
    }
    if ((this->attr) & (Attribute::Static)) {
        preAttrs += "static ";
    }
    if ((this->attr) & (Attribute::Virtual)) {
        preAttrs += "virtual ";
    }
    std::string postAttrs = "";
    if ((this->attr) & (Attribute::Const)) {
        postAttrs += " const ";
    }
    if ((this->parent) != nullptr) {
        return protection + tmp + preAttrs + declaration + sbody + "\n";
    }
    return tmp + preAttrs + declaration + postAttrs + sbody + "\n";
}
std::string IRFunction::GetSource()  const {
    if ((this->IsTemplate()) || body == 0) {
        return "";
    }
    std::string const name = this->GetFullName();
    std::string retType = this->returnType + " ";
    if (this->parent != 0 && (this->parent->GetName()) == this->name) {
        retType = "";
    }
    std::string declaration = retType + name + (this->GetParamenters()) + " ";
    std::string body = "";
    if ((this->body) != nullptr) {
        body = (this->body->ToString()) + "\n";
    }
    return declaration + body;
}
std::string IROperator::GetFullName()  const {
    if ((this->parent) != nullptr) {
        return (this->parent->GetFullName()) + "::" + this->name;
    }
    return this->name;
}
bool IROperator::IsTemplate()  const {
    return (this->templateParams.size()) > 0 || (this->parent) != nullptr && (this->parent->IsTemplate());
}
std::string IROperator::GetTemplate()  const {
    std::string str = "";
    if ((this->templateParams.size()) > 0) {
        str += "template<";
        int i = 0;
        while ((i) < this->templateParams.size()) {
            std::string tType = std::get<0>(this->templateParams[i]);
            if ((tType) == "type") {
                tType = "typename";
            }
            str += tType + " " + std::get<1>(this->templateParams[i]);
            if ((i) < ((this->templateParams.size()) - 1)) {
                str += ", ";
            }
            i++;
        }
        str += "> ";
    }
    return str;
}
std::string IROperator::GetParamenters()  const {
    std::string str = "(";
    int i = 0;
    while ((i) < this->params.size()) {
        str += std::get<0>(this->params[i]) + " " + std::get<1>(this->params[i]);
        if ((i) < ((this->params.size()) - 1)) {
            str += ", ";
        }
        i++;
    }
    return str + ")";
}
std::string IROperator::GetHeader()  const {
    std::string const name = this->name;
    std::string protection = "protected: ";
    if ((this->attr) & (Attribute::Public)) {
        protection = "public: ";
    }
    if ((this->attr) & (Attribute::Private)) {
        protection = "private: ";
    }
    std::string retType = this->returnType + " ";
    if (this->parent != 0 && (this->parent->GetName()) == this->name) {
        retType = "";
    }
    std::string declaration = retType + name + (this->GetParamenters());
    std::string const tmp = this->GetTemplate();
    std::string sbody = ";";
    if ((this->IsTemplate())) {
        if ((this->body) != 0) {
            sbody = " " + (this->body->ToString()) + ";";
        }
    }
    if ((this->body) == 0 && (this->attr) & (Attribute::Virtual)) {
        sbody = " = 0;";
    }
    std::string preAttrs = "";
    if (this->returnType != "void" && this->returnType != "auto") {
        preAttrs += "[[nodiscard]] ";
    }
    if ((this->attr) & (Attribute::Static)) {
        preAttrs += "static ";
    }
    if ((this->attr) & (Attribute::Virtual)) {
        preAttrs += "virtual ";
    }
    std::string postAttrs = "";
    if ((this->attr) & (Attribute::Const)) {
        postAttrs += " const ";
    }
    if ((this->parent) != nullptr) {
        return protection + tmp + preAttrs + declaration + sbody + "\n";
    }
    return tmp + preAttrs + declaration + postAttrs + sbody + "\n";
}
std::string IROperator::GetSource()  const {
    if ((this->IsTemplate()) || body == 0) {
        return "";
    }
    std::string const name = this->GetFullName();
    std::string retType = this->returnType + " ";
    if (this->parent != 0 && (this->parent->GetName()) == this->name) {
        retType = "";
    }
    std::string declaration = retType + name + (this->GetParamenters()) + " ";
    std::string body = "";
    if ((this->body) != nullptr) {
        body = (this->body->ToString()) + "\n";
    }
    return declaration + body;
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
std::string IRVariable::GetFullName()  const {
    if ((this->parent) != nullptr) {
        return (this->parent->GetFullName()) + "::" + this->name;
    }
    return this->name;
}
std::string IRVariable::GetHeader()  const {
    std::string const name = this->name;
    std::string protection = "protected: ";
    if ((this->attr) & (Attribute::Public)) {
        protection = "public: ";
    }
    if ((this->attr) & (Attribute::Private)) {
        protection = "private: ";
    }
    std::string declaration = this->vType + " " + name;
    std::string preAttrs = "";
    if ((this->attr) & (Attribute::Static)) {
        preAttrs += "static ";
    }
    if ((this->attr) & (Attribute::Virtual)) {
        preAttrs += "virtual ";
    }
    std::string value = "";
    if (this->value != 0) {
        value = " = " + (this->value->ToString());
    }
    return protection + preAttrs + declaration + value + ";\n";
}
std::string IRVariable::GetSource()  const {
    return "";
}
std::string IRImport::ToString()  const {
    return "import \"" + this->path + '\"';
}
std::string IRClass::GetName()  const {
    return this->name;
}
std::string IRClass::GetFullName()  const {
    if ((this->parent) != nullptr) {
        return (this->parent->GetFullName()) + "::" + this->name;
    }
    return this->name;
}
bool IRClass::IsTemplate()  const {
    return (this->templateParams.size()) > 0 || (this->parent) != nullptr && (this->parent->IsTemplate());
}
std::string IREnum::ToString()  const {
    std::string str = "enum class " + this->name + " {\n";
    int i = 0;
    while ((i) < this->values.size()) {
        IRExpr* const expr = std::get<1>(this->values[i]);
        std::string value = "";
        if (expr != 0) {
            value = " = " + (expr->ToString());
        }
        str += "    " + (std::get<0>(this->values[i])) + value + ",\n";
        i++;
    }
    return str + "}";
}
std::string IREnum::GetHeader()  const {
    return (this->ToString()) + ";\n";
}
std::string IREnum::GetSource()  const {
    return "";
}
