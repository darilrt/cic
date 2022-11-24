#pragma once
#include "iostream"
#include "vector"
#include "Token.hpp"
class ASTNode {
public: ASTNode();
public: [[nodiscard]] virtual std::string ToString() const ;
};
class ASTConstant : public ASTNode {
public: std::string value;
public: ASTConstant(std::string value);
public: [[nodiscard]] std::string ToString() const ;
};
class ASTVariable : public ASTNode {
public: std::string name;
public: ASTVariable(std::string name);
public: [[nodiscard]] std::string ToString() const ;
};
class ASTBinaryOp : public ASTNode {
public: ASTNode* left;
public: Token op;
public: ASTNode* right;
public: ASTBinaryOp(ASTNode* left, Token op, ASTNode* right);
public: ASTBinaryOp();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTUnaryOp : public ASTNode {
public: Token op;
public: ASTNode* right;
public: ASTUnaryOp(Token op, ASTNode* right);
public: ASTUnaryOp();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTIndexOp : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTIndexOp(ASTNode* left, ASTNode* right);
public: ASTIndexOp();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTFunctionCall : public ASTNode {
public: ASTNode* func;
public: std::vector<ASTNode*> args;
public: ASTFunctionCall(ASTNode* func);
public: ASTFunctionCall(ASTNode* func, std::vector<ASTNode*> args);
public: ASTFunctionCall();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTDotAccess : public ASTNode {
public: ASTNode* left;
public: Token op;
public: ASTNode* right;
public: ASTDotAccess(ASTNode* left, Token op, ASTNode* right);
public: ASTDotAccess();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTScopeResolution : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTScopeResolution(ASTNode* left, ASTNode* right);
public: ASTScopeResolution();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTTemplateArgs : public ASTNode {
public: ASTNode* left;
public: std::vector<ASTNode*> args;
public: ASTTemplateArgs(ASTNode* left);
public: ASTTemplateArgs(ASTNode* left, std::vector<ASTNode*> args);
public: ASTTemplateArgs();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTPointerDecl : public ASTNode {
public: ASTNode* left;
public: ASTPointerDecl(ASTNode* left);
public: ASTPointerDecl();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTReferenceDecl : public ASTNode {
public: ASTNode* left;
public: ASTReferenceDecl(ASTNode* left);
public: ASTReferenceDecl();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTTypeDecl : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTTypeDecl(ASTNode* left, ASTNode* right);
public: ASTTypeDecl();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTBody : public ASTNode {
public: std::vector<ASTNode*> body;
public: ASTBody(std::vector<ASTNode*> body);
public: ASTBody();
public: void AddStatement(ASTNode* stmt);
public: [[nodiscard]] std::string ToString() const ;
};
class ASTType : public ASTNode {
public: ASTNode* left;
public: ASTType(ASTNode* left);
public: ASTType();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTArgDecl : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTArgDecl(ASTNode* left, ASTNode* right);
public: ASTArgDecl();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTTemplateDecl : public ASTNode {
protected: std::vector<ASTNode*> args;
public: ASTTemplateDecl(std::vector<ASTNode*> args);
public: ASTTemplateDecl();
public: void AddArg(ASTNode* arg);
public: [[nodiscard]] std::string ToString() const ;
};
class ASTFunctionDecl : public ASTNode {
public: ASTNode* name;
public: ASTNode* templateDecl;
public: std::vector<ASTNode*> args;
public: ASTNode* _type;
public: ASTNode* body;
public: ASTFunctionDecl(ASTNode* name, ASTNode* templateDecl, std::vector<ASTNode*> args, ASTNode* _type, ASTNode* body);
public: ASTFunctionDecl();
public: [[nodiscard]] std::string ToString() const ;
};
