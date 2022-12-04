#include "AST.hpp"
#include "iostream"
#include "vector"
#include "Common.hpp"
#include "Token.hpp"
ASTNode::ASTNode() {
}
std::string ASTNode::ToString()  const {
    return "ASTNode";
}
std::string ASTNode::Get()  const {
    return "ASTNode";
}
std::string ASTNode::GetType()  const {
    return "ASTNode";
}
ASTProgram::ASTProgram(std::vector<ASTNode*> declList) {
    this->decls = declList;
}
std::string ASTProgram::ToString()  const {
    std::string str = "";
    int i = 0;
    while ((i) < this->decls.size()) {
        str += ((this->decls[i])->ToString());
        if ((i) < ((this->decls.size()) - 1)) {
            str += "\n";
        }
        i += 1;
    }
    return str;
}
std::string ASTProgram::Get()  const {
    return "ASTProgram";
}
std::string ASTProgram::GetType()  const {
    return "ASTProgram";
}
ASTConstant::ASTConstant(std::string value) {
    this->value = value;
}
std::string ASTConstant::ToString()  const {
    return "'" + this->value + "'";
}
std::string ASTConstant::Get()  const {
    return this->value;
}
std::string ASTConstant::GetType()  const {
    return "ASTConstant";
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
std::string ASTVariable::GetType()  const {
    return "ASTVariable";
}
ASTBinaryOp::ASTBinaryOp(ASTNode* left, Token operation, ASTNode* right) {
    this->left = left;
    this->operation = operation;
    this->right = right;
}
ASTBinaryOp::ASTBinaryOp() {
    delete this->left;
    delete this->right;
}
std::string ASTBinaryOp::ToString()  const {
    return "[" + (this->left->ToString()) + " " + (this->operation.ToString()) + " " + (this->right->ToString()) + "]";
}
std::string ASTBinaryOp::Get()  const {
    return (this->left->Get()) + " " + (this->operation.literal) + " " + (this->right->Get());
}
std::string ASTBinaryOp::GetType()  const {
    return "ASTBinaryOp";
}
ASTUnaryOp::ASTUnaryOp(Token operation, ASTNode* right) {
    this->operation = operation;
    this->right = right;
}
ASTUnaryOp::ASTUnaryOp() {
    delete this->right;
}
std::string ASTUnaryOp::ToString()  const {
    return "[" + (this->operation.literal) + " " + (this->right->ToString()) + "]";
}
std::string ASTUnaryOp::Get()  const {
    if (this->isPostfix) {
        return (this->right->Get()) + (this->operation.literal);
    } else {
        return (this->operation.literal) + (this->right->Get());
    }
}
std::string ASTUnaryOp::GetType()  const {
    return "ASTUnaryOp";
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
std::string ASTIndexOp::Get()  const {
    std::string const left = (this->left->Get());
    std::string right;
    if (this->right != 0) {
        right = (this->right->Get());
    }
    return left + "[" + right + "]";
}
std::string ASTIndexOp::GetType()  const {
    return "ASTIndexOp";
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
    std::string str;
    str += (this->func->ToString()) + "(";
    int i = 0;
    while ((i) < this->args.size()) {
        str += (this->args[i])->ToString();
        if ((i) < (this->args.size()) - 1) {
            str += ", ";
        }
        i += 1;
    }
    str += ")";
    return str;
}
std::string ASTFunctionCall::Get()  const {
    std::string str = (this->func->Get()) + "(";
    int i = 0;
    while ((i) < this->args.size()) {
        str += (this->args[i])->Get();
        if ((i) < (this->args.size()) - 1) {
            str += ", ";
        }
        i++;
    }
    str += ")";
    return str;
}
std::string ASTFunctionCall::GetType()  const {
    return "ASTFunctionCall";
}
ASTDotAccess::ASTDotAccess(ASTNode* left, Token operation, ASTNode* right) {
    this->left = left;
    this->operation = operation;
    this->right = right;
}
ASTDotAccess::ASTDotAccess() {
    delete this->left;
    delete this->right;
}
std::string ASTDotAccess::ToString()  const {
    return "[" + (this->left->ToString()) + (this->operation.ToString()) + (this->right->ToString()) + "]";
}
std::string ASTDotAccess::Get()  const {
    return (this->left->Get()) + (this->operation.literal) + (this->right->Get());
}
std::string ASTDotAccess::GetType()  const {
    return "ASTDotAccess";
}
ASTThisAccess::ASTThisAccess(ASTNode* right) {
    this->right = right;
}
ASTThisAccess::ASTThisAccess() {
    delete this->right;
}
std::string ASTThisAccess::ToString()  const {
    return "[this->" + (this->right->ToString()) + "]";
}
std::string ASTThisAccess::Get()  const {
    return "this->" + (this->right->Get());
}
std::string ASTThisAccess::GetType()  const {
    return "ASTThisAccess";
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
std::string ASTScopeResolution::Get()  const {
    return (this->left->Get()) + "::" + (this->right->Get());
}
std::string ASTScopeResolution::GetType()  const {
    return "ASTScopeResolution";
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
    std::string str = (this->left->Get()) + "<";
    int i = 0;
    while ((i) < this->args.size()) {
        str += (this->args[i])->Get();
        if ((i) < (this->args.size()) - 1) {
            str += ", ";
        }
        i++;
    }
    str += ">";
    return str;
}
std::string ASTTemplateArgs::GetType()  const {
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
std::string ASTPointerDecl::Get()  const {
    return (this->left->Get()) + "*";
}
std::string ASTPointerDecl::GetType()  const {
    return "ASTPointerDecl";
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
std::string ASTReferenceDecl::Get()  const {
    return (this->left->Get()) + "&";
}
std::string ASTReferenceDecl::GetType()  const {
    return "ASTReferenceDecl";
}
ASTNew::ASTNew(ASTNode* node) {
    this->node = node;
}
ASTNew::ASTNew() {
    delete this->node;
}
std::string ASTNew::ToString()  const {
    return "new " + (this->node->ToString());
}
std::string ASTNew::Get()  const {
    return "new " + (this->node->Get());
}
std::string ASTNew::GetType()  const {
    return "ASTNew";
}
ASTDel::ASTDel(ASTNode* node, bool isArray) {
    this->node = node;
    this->isArray = isArray;
}
ASTDel::ASTDel() {
    delete this->node;
}
std::string ASTDel::ToString()  const {
    std::string str = "delete ";
    if (this->isArray) {
        str += "[] ";
    }
    str += (this->node->ToString());
    return str;
}
std::string ASTDel::Get()  const {
    std::string str = "delete ";
    if (this->isArray) {
        str += "[] ";
    }
    str += (this->node->Get());
    return str;
}
std::string ASTDel::GetType()  const {
    return "ASTDel";
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
std::string ASTTypeDecl::Get()  const {
    return (this->left->Get()) + " " + (this->right->Get());
}
std::string ASTTypeDecl::GetType()  const {
    return "ASTTypeDecl";
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
            str += "\n";
        }
        i++;
    }
    str += "}";
    return str;
}
std::string ASTBody::Get()  const {
    return "ASTBody";
}
std::string ASTBody::GetType()  const {
    return "ASTBody";
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
    std::string str = "";
    if (this->isConst) {
        str = "const ";
    }
    str += (this->left->Get());
    return str;
}
std::string ASTType::GetType()  const {
    return "ASTType";
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
std::string ASTArgDecl::Get()  const {
    return (this->right->Get()) + " " + (this->left->Get());
}
std::string ASTArgDecl::GetType()  const {
    return "ASTArgDecl";
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
std::string ASTTemplateDecl::Get()  const {
    std::string str = "template<";
    int i = 0;
    while ((i) < this->args.size()) {
        str += (this->args[i])->Get();
        if ((i) < (this->args.size()) - 1) {
            str += ", ";
        }
        i++;
    }
    str += ">";
    return str;
}
std::string ASTTemplateDecl::GetType()  const {
    return "ASTTemplateDecl";
}
ASTExpr::ASTExpr(ASTNode* expr) {
    this->expr = expr;
}
ASTExpr::ASTExpr() {
    delete this->expr;
}
std::string ASTExpr::ToString()  const {
    return (this->expr->ToString());
}
std::string ASTExpr::Get()  const {
    return (this->expr->Get());
}
std::string ASTExpr::GetType()  const {
    return "ASTExpr";
}
ASTOperatorDecl::ASTOperatorDecl(std::string operation, std::vector<ASTArgDecl*> args, ASTType* returnType, ASTBody* body) {
    this->operation = operation;
    this->args = args;
    this->returnType = returnType;
    this->body = body;
}
ASTOperatorDecl::ASTOperatorDecl() {
    int i = 0;
    while ((i) < this->args.size()) {
        delete this->args[i];
    }
    delete this->returnType;
    delete this->body;
}
std::string ASTOperatorDecl::ToString()  const {
    return "ASTOperatorDecl";
}
std::string ASTOperatorDecl::Get()  const {
    return "ASTOperatorDecl";
}
std::string ASTOperatorDecl::GetType()  const {
    return "ASTOperatorDecl";
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
    std::string str = "";
    return str;
}
std::string ASTFunctionDecl::Get()  const {
    return "ASTFunctionDecl";
}
std::string ASTFunctionDecl::GetType()  const {
    return "ASTFunctionDecl";
}
ASTReturn::ASTReturn(ASTExpr* expr) {
    this->expr = expr;
}
ASTReturn::ASTReturn() {
    delete this->expr;
}
std::string ASTReturn::ToString()  const {
    return "return " + (this->expr->ToString()) + ";";
}
std::string ASTReturn::Get()  const {
    return "ASTReturn";
}
std::string ASTReturn::GetType()  const {
    return "ASTReturn";
}
ASTIf::ASTIf(ASTExpr* expr, ASTBody* body) {
    this->expr = expr;
    this->body = body;
}
ASTIf::ASTIf(ASTExpr* expr, ASTBody* body, std::vector<ASTIf*> elifs, ASTBody* elseBody) {
    this->expr = expr;
    this->body = body;
    this->elifs = elifs;
    this->elseBody = elseBody;
}
ASTIf::ASTIf() {
    delete this->expr;
    delete this->body;
    delete this->elseBody;
    int i = 0;
    while ((i) < this->elifs.size()) {
        delete this->elifs[i];
    }
}
std::string ASTIf::ToString()  const {
    return "if (" + (this->expr->ToString()) + ") " + (this->body->ToString());
}
std::string ASTIf::Get()  const {
    return "ASTIf";
}
std::string ASTIf::GetType()  const {
    return "ASTIf";
}
ASTWhile::ASTWhile(ASTExpr* expr, ASTBody* body) {
    this->expr = expr;
    this->body = body;
}
ASTWhile::ASTWhile() {
    delete this->expr;
    delete this->body;
}
std::string ASTWhile::ToString()  const {
    return "while (" + (this->expr->ToString()) + ") " + (this->body->ToString());
}
std::string ASTWhile::Get()  const {
    return "ASTWhile";
}
std::string ASTWhile::GetType()  const {
    return "ASTWhile";
}
std::string ASTBreak::ToString()  const {
    return "break;";
}
std::string ASTBreak::Get()  const {
    return "ASTBreak";
}
std::string ASTBreak::GetType()  const {
    return "ASTBreak";
}
std::string ASTContinue::ToString()  const {
    return "continue;";
}
std::string ASTContinue::Get()  const {
    return "ASTContinue";
}
std::string ASTContinue::GetType()  const {
    return "ASTContinue";
}
ASTVariableDecl::ASTVariableDecl(ASTNode* name, ASTType* vType, ASTExpr* value, bool isMutable) {
    this->name = name;
    this->vType = vType;
    this->value = value;
    this->isMutable = isMutable;
}
ASTVariableDecl::ASTVariableDecl() {
    delete this->name;
    delete this->vType;
    delete this->value;
}
std::string ASTVariableDecl::ToString()  const {
    std::string str = "";
    if (this->isMutable) {
        str += "mut ";
    }
    str += (this->vType->ToString()) + " " + (this->name->ToString());
    if (this->value != 0) {
        str += " = " + (this->value->ToString());
    }
    str += ";";
    return str;
}
std::string ASTVariableDecl::Get()  const {
    return "ASTVariableDecl";
}
std::string ASTVariableDecl::GetType()  const {
    return "ASTVariableDecl";
}
ASTImport::ASTImport(Token* path) {
    this->path = path;
}
std::string ASTImport::ToString()  const {
    return "import " + (this->path->literal) + ";";
}
std::string ASTImport::Get()  const {
    return "ASTImport";
}
std::string ASTImport::GetType()  const {
    return "ASTImport";
}
ASTInherArg::ASTInherArg(Token* protection, ASTNode* name) {
    this->protection = protection;
    this->name = name;
}
std::string ASTInherArg::ToString()  const {
    std::string str = "";
    if (this->protection == 0) {
        str += "protected ";
    } else if (this->protection->tokenType == TokenType::Pub) {
        str += "public ";
    } else if (this->protection->tokenType == TokenType::Priv) {
        str += "private ";
    }
    return str + (this->name->Get());
}
std::string ASTInherArg::Get()  const {
    return "ASTInherArg";
}
std::string ASTInherArg::GetType()  const {
    return "ASTInherArg";
}
ASTClassDecl::ASTClassDecl(ASTNode* name, ASTTemplateDecl* templateDecl, std::vector<ASTInherArg*> inherits, ASTBody* body) {
    this->name = name;
    this->templateDecl = templateDecl;
    this->inherits = inherits;
    this->body = body;
}
ASTClassDecl::ASTClassDecl() {
    delete this->name;
    delete this->templateDecl;
    delete this->body;
    int i = 0;
    while ((i) < this->inherits.size()) {
        delete this->inherits[i];
    }
}
std::string ASTClassDecl::ToString()  const {
    std::string str = "class " + (this->name->ToString());
    if (this->templateDecl != 0) {
        str += (this->templateDecl->ToString());
    }
    if ((this->inherits.size()) > 0) {
        str += " : ";
        int i = 0;
        while ((i) < (this->inherits.size())) {
            str += ((this->inherits[i])->ToString());
            if ((i + 1) < this->inherits.size()) {
                str += ", ";
            }
            i += 1;
        }
    }
    str += (this->body->ToString());
    return str;
}
std::string ASTClassDecl::Get()  const {
    return "ASTClassDecl";
}
std::string ASTClassDecl::GetType()  const {
    return "ASTClassDecl";
}
ASTEnumField::ASTEnumField(ASTNode* name, ASTExpr* value) {
    this->name = name;
    this->value = value;
}
ASTEnumField::ASTEnumField() {
    delete this->name;
    delete this->value;
}
std::string ASTEnumField::ToString()  const {
    std::string str = (this->name->ToString());
    if (this->value != 0) {
        str += " = " + (this->value->ToString());
    }
    return str;
}
std::string ASTEnumField::Get()  const {
    return "ASTEnumField";
}
std::string ASTEnumField::GetType()  const {
    return "ASTEnumField";
}
ASTEnumDecl::ASTEnumDecl(ASTNode* name, std::vector<ASTEnumField*> values) {
    this->name = name;
    this->values = values;
}
ASTEnumDecl::ASTEnumDecl() {
    delete this->name;
    int i = 0;
    while ((i) < this->values.size()) {
        delete this->values[i];
    }
}
std::string ASTEnumDecl::ToString()  const {
    std::string str = "enum " + (this->name->ToString()) + " {\n";
    int i = 0;
    while ((i) < this->values.size()) {
        str += "    " + ((this->values[i])->ToString()) + ",\n";
        i += 1;
    }
    str += "}";
    return str;
}
std::string ASTEnumDecl::Get()  const {
    return "ASTEnumDecl";
}
std::string ASTEnumDecl::GetType()  const {
    return "ASTEnumDecl";
}
