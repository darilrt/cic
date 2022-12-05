#include "IRGenerator.hpp"
#include "iostream"
#include "vector"
#include "tuple"
#include "AST.hpp"
#include "IR.hpp"
IRNode* IRGenerator::Generate(ASTNode* ast) {
    IRNode* const ir = TranslateNode(ast, 0);
    return ir;
}
IRNode* IRGenerator::TranslateNode(ASTNode* node, IRNode* parent) {
    if (IsA<ASTProgram>(node)) {
        return TranslateProgram(As<ASTProgram>(node));
    }
    if (IsA<ASTBody>(node)) {
        return TranslateBody(As<ASTBody>(node), parent);
    }
    if (IsA<ASTFunctionDecl>(node)) {
        return TranslateFunction(As<ASTFunctionDecl>(node), parent);
    }
    if (IsA<ASTOperatorDecl>(node)) {
        return TranslateOperator(As<ASTOperatorDecl>(node), parent);
    }
    if (IsA<ASTVariableDecl>(node)) {
        return TranslateVariable(As<ASTVariableDecl>(node), parent);
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
    if (IsA<ASTWhile>(node)) {
        return TranslateWhile(As<ASTWhile>(node));
    }
    if (IsA<ASTBreak>(node)) {
        return TranslateBreak(As<ASTBreak>(node));
    }
    if (IsA<ASTContinue>(node)) {
        return TranslateContinue(As<ASTContinue>(node));
    }
    if (IsA<ASTImport>(node)) {
        return TranslateImport(As<ASTImport>(node));
    }
    if (IsA<ASTClassDecl>(node)) {
        return TranslateClass(As<ASTClassDecl>(node), parent);
    }
    if (IsA<ASTEnumDecl>(node)) {
        return TranslateEnum(As<ASTEnumDecl>(node), parent);
    }
    if (node != 0) {
        std::cout << "Unknown node type: " << node->GetType() << std::endl;
    } else {
    }
    return 0;
}
IRExpr* IRGenerator::TranslateExpr(ASTExpr* node) {
    if (node == 0) {
        return 0;
    }
    return new IRExpr(node->Get());
}
IRNode* IRGenerator::TranslateProgram(ASTProgram* node) {
    std::vector<IRNode*> decls;
    int i = 0;
    while ((i) < node->decls.size()) {
        decls.push_back(TranslateNode(node->decls[i], 0));
        i++;
    }
    return new IRProgram(decls);
}
IRBody* IRGenerator::TranslateBody(ASTBody* body, IRNode* parent) {
    IRBody* irBody = new IRBody();
    int i = 0;
    while ((i) < body->decls.size()) {
        ASTNode* const decl = body->decls[i];
        IRNode* const irDecl = TranslateNode(decl, parent);
        if (irDecl != 0) {
            irBody->decls.push_back(irDecl);
        }
        i++;
    }
    return irBody;
}
IRNode* IRGenerator::TranslateFunction(ASTFunctionDecl* funcDecl, IRNode* parent) {
    IRFunction* const func = new IRFunction();
    func->name = funcDecl->name->Get();
    func->attr = funcDecl->attr;
    func->parent = parent;
    if (funcDecl->fType != 0) {
        func->returnType = funcDecl->fType->Get();
    } else {
        func->returnType = "auto";
    }
    int i = 0;
    while ((i) < funcDecl->args.size()) {
        ASTArgDecl* const param = funcDecl->args[i];
        std::tuple<std::string, std::string> const irParam = std::make_tuple(param->right->Get(), param->left->Get());
        func->params.push_back(irParam);
        i++;
    }
    ASTTemplateDecl* const tmp = funcDecl->templateDecl;
    if (tmp) {
        i = 0;
        while ((i) < tmp->args.size()) {
            ASTArgDecl* const arg = tmp->args[i];
            std::tuple<std::string, std::string> const irArg = std::make_tuple(arg->right->Get(), arg->left->Get());
            func->templateParams.push_back(irArg);
            i++;
        }
    }
    if (funcDecl->body) {
        func->body = TranslateBody(funcDecl->body, 0);
    } else {
        func->body = 0;
    }
    return func;
}
IRNode* IRGenerator::TranslateOperator(ASTOperatorDecl* opDecl, IRNode* parent) {
    IROperator* const operation = new IROperator();
    operation->name = "operator " + opDecl->operation;
    operation->attr = opDecl->attr;
    operation->parent = parent;
    if (opDecl->returnType != 0) {
        operation->returnType = opDecl->returnType->Get();
    } else {
        operation->returnType = "auto";
    }
    int i = 0;
    while ((i) < opDecl->args.size()) {
        ASTArgDecl* const param = opDecl->args[i];
        std::tuple<std::string, std::string> const irParam = std::make_tuple(param->right->Get(), param->left->Get());
        operation->params.push_back(irParam);
        i++;
    }
    if (opDecl->body) {
        operation->body = TranslateBody(opDecl->body, 0);
    } else {
        operation->body = 0;
    }
    return operation;
}
IRReturn* IRGenerator::TranslateReturn(ASTReturn* r) {
    IRReturn* const irRet = new IRReturn();
    irRet->expr = TranslateExpr(r->expr);
    return irRet;
}
IRNode* IRGenerator::TranslateVariable(ASTVariableDecl* varDecl, IRNode* parent) {
    IRVariable* const var = new IRVariable();
    var->name = varDecl->name->Get();
    var->vType = varDecl->vType->Get();
    var->parent = parent;
    var->attr = varDecl->attr;
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
    irIf->body = TranslateBody(ifNode->body, 0);
    int i = 0;
    while ((i) < ifNode->elifs.size()) {
        ASTIf* const _elif = ifNode->elifs[i];
        IRIf* const irElif = new IRIf();
        irElif->cond = TranslateExpr(_elif->expr);
        irElif->body = TranslateBody(_elif->body, 0);
        irIf->elifs.push_back(irElif);
        i++;
    }
    if (ifNode->elseBody != 0) {
        irIf->elseBody = TranslateBody(ifNode->elseBody, 0);
    }
    return irIf;
}
IRWhile* IRGenerator::TranslateWhile(ASTWhile* whileNode) {
    IRWhile* const irWhile = new IRWhile();
    irWhile->cond = TranslateExpr(whileNode->expr);
    irWhile->body = TranslateBody(whileNode->body, 0);
    return irWhile;
}
IRBreak* IRGenerator::TranslateBreak(ASTBreak* breakNode) {
    return new IRBreak();
}
IRContinue* IRGenerator::TranslateContinue(ASTContinue* continueNode) {
    return new IRContinue();
}
IRImport* IRGenerator::TranslateImport(ASTImport* importNode) {
    IRImport* const irImport = new IRImport();
    irImport->path = importNode->path->literal;
    return irImport;
}
IRClass* IRGenerator::TranslateClass(ASTClassDecl* classNode, IRNode* parent) {
    IRClass* const irClass = new IRClass();
    irClass->name = classNode->name->Get();
    irClass->body = TranslateBody(classNode->body, 0);
    irClass->parent = parent;
    int i = 0;
    while ((i) < classNode->inherits.size()) {
        ASTInherArg* const inherit = classNode->inherits[i];
        irClass->inherits.push_back(inherit->ToString());
        i++;
    }
    ASTTemplateDecl* const tmp = classNode->templateDecl;
    if (tmp) {
        i = 0;
        while ((i) < tmp->args.size()) {
            ASTArgDecl* const arg = tmp->args[i];
            std::tuple<std::string, std::string> const irArg = std::make_tuple(arg->right->Get(), arg->left->Get());
            irClass->templateParams.push_back(irArg);
            i++;
        }
    }
    irClass->body = TranslateBody(classNode->body, irClass);
    return irClass;
}
IREnum* IRGenerator::TranslateEnum(ASTEnumDecl* enumNode, IRNode* parent) {
    IREnum* const irEnum = new IREnum();
    irEnum->name = enumNode->name->Get();
    irEnum->parent = parent;
    int i = 0;
    while ((i) < enumNode->values.size()) {
        ASTEnumField* value = enumNode->values[i];
        irEnum->values.push_back(std::make_tuple(value->name->Get(), TranslateExpr(value->value)));
        i++;
    }
    return irEnum;
}
