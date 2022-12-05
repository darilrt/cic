#pragma once
#include "iostream"
#include "vector"
#include "Common.bic"
#include "Token.bic"
class ASTNode {
public: int attr = 0;
public: ASTNode();
public: [[nodiscard]] virtual std::string ToString();
public: [[nodiscard]] virtual std::string Get();
public: [[nodiscard]] virtual std::string GetType();
};
class ASTProgram : public ASTNode {
public: std::vector<ASTNode*> decls;
public: ASTProgram(std::vector<ASTNode*> declList);
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTConstant : public ASTNode {
public: std::string value;
public: ASTConstant(std::string value);
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTVariable : public ASTNode {
public: std::string name;
public: ASTVariable(std::string name);
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTBinaryOp : public ASTNode {
public: ASTNode* left;
public: Token operation;
public: ASTNode* right;
public: ASTBinaryOp(ASTNode* left, Token operation, ASTNode* right);
public: ~ASTBinaryOp();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTUnaryOp : public ASTNode {
public: Token operation;
public: ASTNode* right;
public: bool isPostfix = false;
public: ASTUnaryOp(Token operation, ASTNode* right);
public: ~ASTUnaryOp();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTIndexOp : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTIndexOp(ASTNode* left, ASTNode* right);
public: ~ASTIndexOp();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTFunctionCall : public ASTNode {
public: ASTNode* func;
public: std::vector<ASTNode*> args;
public: ASTFunctionCall(ASTNode* func);
public: ASTFunctionCall(ASTNode* func, std::vector<ASTNode*> args);
public: ~ASTFunctionCall();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTDotAccess : public ASTNode {
public: ASTNode* left;
public: Token operation;
public: ASTNode* right;
public: ASTDotAccess(ASTNode* left, Token operation, ASTNode* right);
public: ~ASTDotAccess();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTThisAccess : public ASTNode {
public: ASTNode* right;
public: ASTThisAccess(ASTNode* right);
public: ~ASTThisAccess();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTScopeResolution : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTScopeResolution(ASTNode* left, ASTNode* right);
public: ~ASTScopeResolution();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTTemplateArgs : public ASTNode {
public: ASTNode* left;
public: std::vector<ASTNode*> args;
public: ASTTemplateArgs(ASTNode* left);
public: ASTTemplateArgs(ASTNode* left, std::vector<ASTNode*> args);
public: ~ASTTemplateArgs();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTPointerDecl : public ASTNode {
public: ASTNode* left;
public: ASTPointerDecl(ASTNode* left);
public: ~ASTPointerDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTReferenceDecl : public ASTNode {
public: ASTNode* left;
public: ASTReferenceDecl(ASTNode* left);
public: ~ASTReferenceDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTNew : public ASTNode {
public: ASTNode* node;
public: ASTNew(ASTNode* node);
public: ~ASTNew();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTDel : public ASTNode {
public: ASTNode* node;
public: bool isArray;
public: ASTDel(ASTNode* node, bool isArray);
public: ~ASTDel();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTTypeDecl : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTTypeDecl(ASTNode* left, ASTNode* right);
public: ~ASTTypeDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTBody : public ASTNode {
public: std::vector<ASTNode*> decls;
public: ASTBody(std::vector<ASTNode*> decls);
public: ~ASTBody();
public: void AddStatement(ASTNode* stmt);
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTType : public ASTNode {
public: ASTNode* left;
public: bool isConst = false;
public: ASTType(ASTNode* left);
public: ~ASTType();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTArgDecl : public ASTNode {
public: ASTNode* left;
public: ASTNode* right;
public: ASTArgDecl(ASTNode* left, ASTNode* right);
public: ~ASTArgDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTTemplateDecl : public ASTNode {
public: std::vector<ASTArgDecl*> args;
public: ASTTemplateDecl(std::vector<ASTArgDecl*> args);
public: ~ASTTemplateDecl();
public: void AddArg(ASTArgDecl* arg);
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTExpr : public ASTNode {
public: ASTNode* expr;
public: ASTExpr(ASTNode* expr);
public: ~ASTExpr();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTOperatorDecl : public ASTNode {
public: std::string operation;
public: std::vector<ASTArgDecl*> args;
public: ASTType* returnType;
public: ASTBody* body;
public: ASTOperatorDecl(std::string operation, std::vector<ASTArgDecl*> args, ASTType* returnType, ASTBody* body);
public: ~ASTOperatorDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTFunctionDecl : public ASTNode {
public: ASTVariable* name;
public: ASTTemplateDecl* templateDecl;
public: std::vector<ASTArgDecl*> args;
public: ASTType* fType;
public: ASTBody* body;
public: ASTFunctionDecl(ASTVariable* name, ASTTemplateDecl* templateDecl, std::vector<ASTArgDecl*> args, ASTType* fType, ASTBody* body);
public: ~ASTFunctionDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTReturn : public ASTNode {
public: ASTExpr* expr;
public: ASTReturn(ASTExpr* expr);
public: ~ASTReturn();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTIf : public ASTNode {
public: ASTExpr* expr;
public: ASTBody* body;
public: std::vector<ASTIf*> elifs;
public: ASTBody* elseBody;
public: ASTIf(ASTExpr* expr, ASTBody* body);
public: ASTIf(ASTExpr* expr, ASTBody* body, std::vector<ASTIf*> elifs, ASTBody* elseBody);
public: ~ASTIf();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTWhile : public ASTNode {
public: ASTExpr* expr;
public: ASTBody* body;
public: ASTWhile(ASTExpr* expr, ASTBody* body);
public: ~ASTWhile();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTBreak : public ASTNode {
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTContinue : public ASTNode {
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTVariableDecl : public ASTNode {
public: ASTNode* name;
public: ASTType* vType;
public: ASTExpr* value;
public: bool isMutable;
public: ASTVariableDecl(ASTNode* name, ASTType* vType, ASTExpr* value, bool isMutable);
public: ~ASTVariableDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTImport : public ASTNode {
public: Token* path;
public: ASTImport(Token* path);
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTInherArg : public ASTNode {
public: Token* protection;
public: ASTNode* name;
public: ASTInherArg(Token* protection, ASTNode* name);
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTClassDecl : public ASTNode {
public: ASTNode* name;
public: ASTTemplateDecl* templateDecl;
public: std::vector<ASTInherArg*> inherits;
public: ASTBody* body;
public: ASTClassDecl(ASTNode* name, ASTTemplateDecl* templateDecl, std::vector<ASTInherArg*> inherits, ASTBody* body);
public: ~ASTClassDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTEnumField : public ASTNode {
public: ASTNode* name;
public: ASTExpr* value;
public: ASTEnumField(ASTNode* name, ASTExpr* value);
public: ~ASTEnumField();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
class ASTEnumDecl : public ASTNode {
public: ASTNode* name;
public: std::vector<ASTEnumField*> values;
public: ASTEnumDecl(ASTNode* name, std::vector<ASTEnumField*> values);
public: ~ASTEnumDecl();
public: [[nodiscard]] std::string ToString();
public: [[nodiscard]] std::string Get();
public: [[nodiscard]] std::string GetType();
};
