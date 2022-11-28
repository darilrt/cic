#include "AST.hpp"
#include "iostream"
#include "vector"
#include "Token.hpp"
ASTNode::ASTNode() {
}
std::string ASTNode::ToString()  const {
    return "ASTNode";
}
std::string ASTNode::Get()  const {
    return "ASTNode";
}
ASTProgram::ASTProgram(std::vector<ASTNode*> declList) {
    this->decls = declList;
}
std::string ASTProgram::ToString()  const {
    std::string str = "";
    int i = 0;
    while ((i) < this->decls.size()) {
        str += ((this->decls[i])->ToString()) + "\n";
        if ((i) < ((this->decls.size()) - 1)) {
            str += "\n";
        }
        i++;
    }
    return str;
}
ASTConstant::ASTConstant(std::string value) {
    this->value = value;
}
std::string ASTConstant::ToString()  const {
    return "'" + this->value + "'";
}
ASTVariable::ASTVariable(std::string name) {
    this->name = name;
}
std::string ASTVariable::ToString()  const {
    return "'" + this->name + "'";
}
std::string ASTVariable::Get()  const {
    return this->name;
}
ASTBinaryOp::ASTBinaryOp(ASTNode* left, Token op, ASTNode* right) {
    this->left = left;
    this->op = op;
    this->right = right;
}
ASTBinaryOp::ASTBinaryOp() {
    delete this->left;
    delete this->right;
}
std::string ASTBinaryOp::ToString()  const {
    return "[" + (this->left->ToString()) + " " + (this->op.ToString()) + " " + (this->right->ToString()) + "]";
}
ASTUnaryOp::ASTUnaryOp(Token op, ASTNode* right) {
    this->op = op;
    this->right = right;
}
ASTUnaryOp::ASTUnaryOp() {
    delete this->right;
}
std::string ASTUnaryOp::ToString()  const {
    return "[" + (this->op.ToString()) + " " + (this->right->ToString()) + "]";
}
ASTIndexOp::ASTIndexOp(ASTNode* left, ASTNode* right) {
    this->left = left;
    this->right = right;
}
ASTIndexOp::ASTIndexOp() {
    delete this->left;
    delete this->right;
}
std::string ASTIndexOp::ToString()  const {
    return "[" + (this->left->ToString()) + "[" + (this->right->ToString()) + "]]";
}
ASTFunctionCall::ASTFunctionCall(ASTNode* func) {
    this->func = func;
}
ASTFunctionCall::ASTFunctionCall(ASTNode* func, std::vector<ASTNode*> args) {
    this->func = func;
    this->args = args;
}
ASTFunctionCall::ASTFunctionCall() {
    delete this->func;
    int i = 0;
    while ((i) < this->args.size()) {
        delete this->args[i];
    }
}
std::string ASTFunctionCall::ToString()  const {
    std::string str = (this->func->ToString()) + "(";
    int i = 0;
    while ((i) < this->args.size()) {
        str += (this->args[i])->ToString();
        if ((i) < (this->args.size()) - 1) {
            str += ", ";
        }
        i++;
    }
    str += ")";
    return str;
}
ASTDotAccess::ASTDotAccess(ASTNode* left, Token op, ASTNode* right) {
    this->left = left;
    this->op = op;
    this->right = right;
}
ASTDotAccess::ASTDotAccess() {
    delete this->left;
    delete this->right;
}
std::string ASTDotAccess::ToString()  const {
    return "[" + (this->left->ToString()) + (this->op.ToString()) + (this->right->ToString()) + "]";
}
ASTScopeResolution::ASTScopeResolution(ASTNode* left, ASTNode* right) {
    this->left = left;
    this->right = right;
}
ASTScopeResolution::ASTScopeResolution() {
    delete this->left;
    delete this->right;
}
std::string ASTScopeResolution::ToString()  const {
    return "[" + (this->left->ToString()) + "::" + (this->right->ToString()) + "]";
}
ASTTemplateArgs::ASTTemplateArgs(ASTNode* left) {
    this->left = left;
}
ASTTemplateArgs::ASTTemplateArgs(ASTNode* left, std::vector<ASTNode*> args) {
    this->left = left;
    this->args = args;
}
ASTTemplateArgs::ASTTemplateArgs() {
    delete this->left;
    int i = 0;
    while ((i) < this->args.size()) {
        delete this->args[i];
    }
}
std::string ASTTemplateArgs::ToString()  const {
    std::string str = (this->left->ToString()) + "<";
    int i = 0;
    while ((i) < this->args.size()) {
        str += (this->args[i])->ToString();
        if ((i) < (this->args.size()) - 1) {
            str += ", ";
        }
        i++;
    }
    str += ">";
    return str;
}
std::string ASTTemplateArgs::Get()  const {
    return "ASTTemplateArgs";
}
ASTPointerDecl::ASTPointerDecl(ASTNode* left) {
    this->left = left;
}
ASTPointerDecl::ASTPointerDecl() {
    delete this->left;
}
std::string ASTPointerDecl::ToString()  const {
    return (this->left->ToString()) + "*";
}
ASTReferenceDecl::ASTReferenceDecl(ASTNode* left) {
    this->left = left;
}
ASTReferenceDecl::ASTReferenceDecl() {
    delete this->left;
}
std::string ASTReferenceDecl::ToString()  const {
    return (this->left->ToString()) + "&";
}
ASTTypeDecl::ASTTypeDecl(ASTNode* left, ASTNode* right) {
    this->left = left;
    this->right = right;
}
ASTTypeDecl::ASTTypeDecl() {
    delete this->left;
    delete this->right;
}
std::string ASTTypeDecl::ToString()  const {
    return (this->left->ToString()) + " " + (this->right->ToString());
}
ASTBody::ASTBody(std::vector<ASTNode*> decls) {
    this->decls = decls;
}
ASTBody::ASTBody() {
    int i = 0;
    while ((i) < this->decls.size()) {
        delete this->decls[i];
    }
}
void ASTBody::AddStatement(ASTNode* stmt) {
    this->decls.push_back(stmt);
}
std::string ASTBody::ToString()  const {
    std::string str = "{";
    int i = 0;
    while ((i) < this->decls.size()) {
        str += (this->decls[i])->ToString();
        if ((i) < (this->decls.size()) - 1) {
            str += ", ";
        }
    }
    str += "}";
    return str;
}
ASTType::ASTType(ASTNode* left) {
    this->left = left;
}
ASTType::ASTType() {
    delete this->left;
}
std::string ASTType::ToString()  const {
    return (this->left->ToString());
}
std::string ASTType::Get()  const {
    return (this->left->Get());
}
ASTArgDecl::ASTArgDecl(ASTNode* left, ASTNode* right) {
    this->left = left;
    this->right = right;
}
ASTArgDecl::ASTArgDecl() {
    delete this->left;
    delete this->right;
}
std::string ASTArgDecl::ToString()  const {
    return (this->right->ToString()) + " " + (this->left->ToString());
}
ASTTemplateDecl::ASTTemplateDecl(std::vector<ASTArgDecl*> args) {
    this->args = args;
}
ASTTemplateDecl::ASTTemplateDecl() {
    int i = 0;
    while ((i) < this->args.size()) {
        delete this->args[i];
    }
}
void ASTTemplateDecl::AddArg(ASTArgDecl* arg) {
    this->args.push_back(arg);
}
std::string ASTTemplateDecl::ToString()  const {
    std::string str = "template<";
    int i = 0;
    while ((i) < this->args.size()) {
        str += (this->args[i])->ToString();
        if ((i) < (this->args.size()) - 1) {
            str += ", ";
        }
        i++;
    }
    str += ">";
    return str;
}
ASTFunctionDecl::ASTFunctionDecl(ASTVariable* name, ASTTemplateDecl* templateDecl, std::vector<ASTArgDecl*> args, ASTType* fType, ASTBody* body) {
    this->name = name;
    this->templateDecl = templateDecl;
    this->args = args;
    this->fType = fType;
    this->body = body;
}
ASTFunctionDecl::ASTFunctionDecl() {
    delete this->name;
    delete this->templateDecl;
    int i = 0;
    while ((i) < this->args.size()) {
        delete this->args[i];
    }
    delete this->fType;
    delete this->body;
}
std::string ASTFunctionDecl::ToString()  const {
    std::string str = (this->templateDecl->ToString()) + " " + (this->fType->ToString()) + " " + (this->name->ToString()) + "(";
    int i = 0;
    while ((i) < this->args.size()) {
        str += (this->args[i])->ToString();
        if ((i) < (this->args.size()) - 1) {
            str += ", ";
        }
        i++;
    }
    str += ") " + (this->body->ToString());
    return str;
}
