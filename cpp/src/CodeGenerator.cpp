#include "CodeGenerator.hpp"
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
        str += ((this->decls[i])->ToString()) + "\n";
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
std::string IRFunction::ToString()  const {
    std::string str = "";
    if ((this->attr & FunctionAttr::Static) != 0) {
        str += "static ";
    }
    if ((this->attr & FunctionAttr::Virtual) != 0) {
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
    if ((this->attr & FunctionAttr::Const) != 0) {
        str += " const";
    }
    str += " ";
    str += this->body->ToString();
    return str;
}
std::tuple<std::string, std::string> CodeGenerator::Generate(ASTNode* ast) {
    std::cout << "Generating code..." << std::endl;
    IRNode* const ir = TranslateNode(ast);
    std::string code = ir->ToString();
    std::string header = "";
    return std::make_tuple(code, header);
}
IRNode* CodeGenerator::TranslateNode(ASTNode* node) {
    if (IsA<ASTProgram>(node)) {
        return TranslateProgram(As<ASTProgram>(node));
    }
    if (IsA<ASTBody>(node)) {
        return TranslateBody(As<ASTBody>(node));
    }
    if (IsA<ASTFunctionDecl>(node)) {
        return TranslateFunction(As<ASTFunctionDecl>(node));
    }
    std::cout << "Unknown node type: " << node->ToString() << std::endl;
    return 0;
}
IRNode* CodeGenerator::TranslateProgram(ASTProgram* node) {
    std::vector<IRNode*> decls;
    int i = 0;
    while ((i) < node->decls.size()) {
        decls.push_back(TranslateNode(node->decls[i]));
        i++;
    }
    return new IRProgram(decls);
}
IRBody* CodeGenerator::TranslateBody(ASTBody* body) {
    IRBody* irBody = new IRBody();
    int i = 0;
    while ((i) < body->decls.size()) {
        ASTNode* const decl = body->decls[i];
        IRNode* const irDecl = TranslateNode(decl);
        irBody->decls.push_back(irDecl);
        i++;
    }
    return irBody;
}
IRNode* CodeGenerator::TranslateFunction(ASTFunctionDecl* funcDecl) {
    IRFunction* const func = new IRFunction();
    func->name = funcDecl->name->Get();
    func->returnType = funcDecl->fType->Get();
    int i = 0;
    while ((i) < funcDecl->args.size()) {
        ASTArgDecl* const param = funcDecl->args[i];
        std::tuple<std::string, std::string> const irParam = std::make_tuple(param->right->Get(), param->left->Get());
        func->params.push_back(irParam);
        i++;
    }
    func->body = TranslateBody(funcDecl->body);
    return func;
}
