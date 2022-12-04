#pragma once
#include "vector"
#include "Token.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
class ParserState {
public: std::string state;
public: Token* token;
public: int current;
public: ParserState(std::string state, int current);
};
enum class BodyType {
    Decl,
    Impl,
    Class,
};
class Parser {
protected: Lexer* lexer;
protected: std::vector<Token> tokens;
protected: int current;
protected: Token* currentToken;
protected: Token* prevToken;
protected: std::vector<ParserState> stateStack;
protected: std::string state;
public: Parser(Lexer* lexer);
public: [[nodiscard]] ASTNode* Parse();
protected: void PushState(std::string state);
protected: void PopState(bool drop);
protected: void ShowState();
protected: [[nodiscard]] bool IsInsideState(std::string state);
protected: void Eat(TokenType tokenType);
protected: void EatNewLine();
protected: void Backtrack();
protected: void Error(std::string msg);
protected: [[nodiscard]] bool IsAtEnd();
protected: [[nodiscard]] bool IsType(TokenType tokenType);
protected: [[nodiscard]] bool IsKeyword();
protected: [[nodiscard]] bool IsState(std::string state);
protected: [[nodiscard]] ASTNode* Program();
protected: [[nodiscard]] ASTExpr* Expr();
protected: [[nodiscard]] ASTNode* AssignExpr();
protected: [[nodiscard]] ASTNode* CompExpr();
protected: [[nodiscard]] ASTNode* ShiftExpr();
protected: [[nodiscard]] ASTNode* Term();
protected: [[nodiscard]] ASTNode* BitwiseExpr();
protected: [[nodiscard]] ASTNode* Factor();
protected: [[nodiscard]] ASTNode* IncDecExpr();
protected: [[nodiscard]] ASTNode* Unary();
protected: [[nodiscard]] ASTNode* FunctionCall();
protected: [[nodiscard]] ASTNode* DotAccess();
protected: [[nodiscard]] ASTNode* IndexAccess(ASTNode* node);
protected: [[nodiscard]] ASTNode* Primary();
protected: [[nodiscard]] ASTNode* New();
protected: [[nodiscard]] ASTNode* Del();
protected: [[nodiscard]] ASTNode* IdentDecl();
protected: [[nodiscard]] ASTNode* Variable();
protected: [[nodiscard]] ASTNode* ScopeResolution();
protected: [[nodiscard]] ASTTemplateArgs* TemplateArgs(ASTNode* node);
protected: [[nodiscard]] std::vector<ASTNode*> ArgsValue();
protected: [[nodiscard]] ASTBody* Body(BodyType bodyType);
protected: [[nodiscard]] ASTType* Type();
protected: [[nodiscard]] ASTArgDecl* ArgDecl();
protected: [[nodiscard]] std::vector<ASTArgDecl*> TypedArgs();
protected: [[nodiscard]] ASTTemplateDecl* TemplateDecl();
protected: [[nodiscard]] ASTVariableDecl* VariableDecl();
protected: [[nodiscard]] ASTFunctionDecl* FunctionDecl();
protected: [[nodiscard]] ASTReturn* ReturnStmt();
protected: [[nodiscard]] ASTIf* IfStmt();
protected: [[nodiscard]] ASTWhile* WhileStmt();
protected: [[nodiscard]] ASTImport* ImportStmt();
protected: [[nodiscard]] ASTInherArg* InherArg();
protected: [[nodiscard]] std::vector<ASTInherArg*> InherArgs();
protected: [[nodiscard]] ASTClassDecl* ClassDecl();
protected: [[nodiscard]] std::string Operator();
protected: [[nodiscard]] ASTOperatorDecl* OperatorDecl();
protected: [[nodiscard]] ASTEnumField* EnumField();
protected: [[nodiscard]] std::vector<ASTEnumField*> EnumFields();
protected: [[nodiscard]] ASTEnumDecl* EnumDecl();
protected: [[nodiscard]] ASTNode* DeclStatement();
protected: [[nodiscard]] ASTNode* ImplStatement();
protected: [[nodiscard]] ASTNode* ClassStatement();
};
