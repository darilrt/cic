#pragma once
#include "iostream"
#include "vector"
#include "tuple"
#include "AST.hpp"
#include "IR.hpp"
template <typename T> [[nodiscard]] bool IsA(ASTNode* node){
    return (dynamic_cast<T*>(node)) != 0;
}
template <typename T> [[nodiscard]] T* As(ASTNode* node){
    return dynamic_cast<T*>(node);
}
class IRGenerator {
public: [[nodiscard]] IRNode* Generate(ASTNode* ast);
protected: [[nodiscard]] IRNode* TranslateNode(ASTNode* node);
protected: [[nodiscard]] IRExpr* TranslateExpr(ASTExpr* node);
protected: [[nodiscard]] IRNode* TranslateProgram(ASTProgram* node);
protected: [[nodiscard]] IRBody* TranslateBody(ASTBody* body);
protected: [[nodiscard]] IRNode* TranslateFunction(ASTFunctionDecl* funcDecl);
protected: [[nodiscard]] IRReturn* TranslateReturn(ASTReturn* r);
protected: [[nodiscard]] IRNode* TranslateVariable(ASTVariableDecl* varDecl);
protected: [[nodiscard]] IRIf* TranslateIf(ASTIf* ifNode);
protected: [[nodiscard]] IRWhile* TranslateWhile(ASTWhile* whileNode);
protected: [[nodiscard]] IRBreak* TranslateBreak(ASTBreak* breakNode);
protected: [[nodiscard]] IRContinue* TranslateContinue(ASTContinue* continueNode);
protected: [[nodiscard]] IRImport* TranslateImport(ASTImport* importNode);
protected: [[nodiscard]] IRClass* TranslateClass(ASTClassDecl* classNode);
};
