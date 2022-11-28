#pragma once
#include "iostream"
#include "vector"
#include "tuple"
#include "AST.hpp"
class IRNode {
public: IRNode();
public: [[nodiscard]] virtual std::string ToString() const ;
};
class IRProgram : public IRNode {
public: std::vector<IRNode*> decls;
public: IRProgram(std::vector<IRNode*> declList);
public: [[nodiscard]] std::string ToString() const ;
};
class IRBody : public IRNode {
public: std::vector<IRNode*> decls;
public: auto IRProgram();
public: [[nodiscard]] std::string ToString() const ;
};
class FunctionAttr {
public: static int const None = 0;
public: static int const Static = 1;
public: static int const Virtual = 2;
public: static int const Const = 4;
};
class IRFunction : public IRNode {
public: std::string name;
public: std::string returnType;
public: std::vector<std::tuple<std::string, std::string>> params;
public: IRBody* body;
public: int attr = FunctionAttr::None;
public: [[nodiscard]] std::string ToString() const ;
};
template <typename T> [[nodiscard]] bool IsA(ASTNode* node){
    return (dynamic_cast<T*>(node)) != 0;
}
template <typename T> [[nodiscard]] T* As(ASTNode* node){
    return dynamic_cast<T*>(node);
}
class CodeGenerator {
public: [[nodiscard]] std::tuple<std::string, std::string> Generate(ASTNode* ast);
protected: [[nodiscard]] IRNode* TranslateNode(ASTNode* node);
protected: [[nodiscard]] IRNode* TranslateProgram(ASTProgram* node);
protected: [[nodiscard]] IRBody* TranslateBody(ASTBody* body);
protected: [[nodiscard]] IRNode* TranslateFunction(ASTFunctionDecl* funcDecl);
};
