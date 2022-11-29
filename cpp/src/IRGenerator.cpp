#include "IRGenerator.hpp"
#include "iostream"
#include "vector"
#include "tuple"
#include "AST.hpp"
#include "IR.hpp"
IRNode* IRGenerator::Generate(ASTNode* ast) {
    IRNode* const ir = TranslateNode(ast);
    return ir;
}
IRNode* IRGenerator::TranslateNode(ASTNode* node) {
    if (IsA<ASTProgram>(node)) {
        return TranslateProgram(As<ASTProgram>(node));
    }
    if (IsA<ASTBody>(node)) {
        return TranslateBody(As<ASTBody>(node));
    }
    if (IsA<ASTFunctionDecl>(node)) {
        return TranslateFunction(As<ASTFunctionDecl>(node));
    }
    if (IsA<ASTVariableDecl>(node)) {
        return TranslateVariable(As<ASTVariableDecl>(node));
    }
    if (IsA<ASTExpr>(node)) {
        return TranslateExpr(As<ASTExpr>(node));
    }
    if (IsA<ASTReturn>(node)) {
        return TranslateReturn(As<ASTReturn>(node));
    }
    if (IsA<ASTIf>(node)) {
        return TranslateIf(As<ASTIf>(node));
    }
    if (node != 0) {
        std::cout << "Unknown node type: " << node->GetType() << std::endl;
    } else {
        std::cout << "Null node" << std::endl;
    }
    return 0;
}
IRExpr* IRGenerator::TranslateExpr(ASTExpr* node) {
    return new IRExpr(node->Get());
}
IRNode* IRGenerator::TranslateProgram(ASTProgram* node) {
    std::vector<IRNode*> decls;
    int i = 0;
    while ((i) < node->decls.size()) {
        decls.push_back(TranslateNode(node->decls[i]));
        i++;
    }
    return new IRProgram(decls);
}
IRBody* IRGenerator::TranslateBody(ASTBody* body) {
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
IRNode* IRGenerator::TranslateFunction(ASTFunctionDecl* funcDecl) {
    IRFunction* const func = new IRFunction();
    func->name = funcDecl->name->Get();
    func->returnType = funcDecl->fType->Get();
    func->attr = funcDecl->attr;
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
IRReturn* IRGenerator::TranslateReturn(ASTReturn* r) {
    IRReturn* const irRet = new IRReturn();
    irRet->expr = TranslateExpr(r->expr);
    return irRet;
}
IRNode* IRGenerator::TranslateVariable(ASTVariableDecl* varDecl) {
    IRVariable* const var = new IRVariable();
    var->name = varDecl->name->Get();
    var->vType = varDecl->vType->Get();
    if (!varDecl->isMutable) {
        var->attr |= Attribute::Const;
    }
    if (varDecl->value != 0) {
        var->value = TranslateExpr(varDecl->value);
    }
    return var;
}
IRIf* IRGenerator::TranslateIf(ASTIf* ifNode) {
    IRIf* const irIf = new IRIf();
    irIf->cond = TranslateExpr(ifNode->expr);
    irIf->body = TranslateBody(ifNode->body);
    int i = 0;
    while ((i) < ifNode->elifs.size()) {
        ASTIf* const _elif = ifNode->elifs[i];
        IRIf* const irElif = new IRIf();
        irElif->cond = TranslateExpr(_elif->expr);
        irElif->body = TranslateBody(_elif->body);
        irIf->elifs.push_back(irElif);
        i++;
    }
    if (ifNode->elseBody != 0) {
        irIf->elseBody = TranslateBody(ifNode->elseBody);
    }
    return irIf;
}
