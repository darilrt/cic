#pragma once
#include "iostream"
#include "vector"
#include "Token.hpp"
class ASTNode {
public: ASTNode();
public: [[nodiscard]] virtual std::string ToString() const ;
public: [[nodiscard]] virtual std::string Get() const ;
};
class ASTProgram : public ASTNode {
public: std::vector<ASTNode*> decls;
public: ASTProgram(std::vector<ASTNode*> declList);
public: [[nodiscard]] std::string ToString() const ;
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
public: [[nodiscard]] std::string Get() const ;
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
public: [[nodiscard]] std::string Get() const ;
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
public: std::vector<ASTNode*> decls;
public: ASTBody(std::vector<ASTNode*> decls);
public: ASTBody();
public: void AddStatement(ASTNode* stmt);
public: [[nodiscard]] std::string ToString() const ;
};
class ASTType : public ASTNode {
public: ASTNode* left;
public: ASTType(ASTNode* left);
public: ASTType();
public: [[nodiscard]] std::string ToString() const ;
public: [[nodiscard]] std::string Get() const ;
};
class ASTArgDecl : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTArgDecl(ASTNode* left, ASTNode* right);
public: ASTArgDecl();
public: [[nodiscard]] std::string ToString() const ;
};
class ASTTemplateDecl : public ASTNode {
protected: std::vector<ASTArgDecl*> args;
public: ASTTemplateDecl(std::vector<ASTArgDecl*> args);
public: ASTTemplateDecl();
public: void AddArg(ASTArgDecl* arg);
public: [[nodiscard]] std::string ToString() const ;
};
class ASTFunctionDecl : public ASTNode {
public: ASTVariable* name;
public: ASTNode* templateDecl;
public: std::vector<ASTArgDecl*> args;
public: ASTType* fType;
public: ASTBody* body;
public: ASTFunctionDecl(ASTVariable* name, ASTTemplateDecl* templateDecl, std::vector<ASTArgDecl*> args, ASTType* fType, ASTBody* body);
public: ASTFunctionDecl();
public: [[nodiscard]] std::string ToString() const ;
};
