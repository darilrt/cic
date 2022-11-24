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
class Parser {
protected: Lexer* lexer;
protected: std::vector<Token> tokens;
protected: int current;
protected: Token* currentToken;
protected: std::vector<ParserState> stateStack;
protected: std::string state;
public: Parser(Lexer* lexer);
protected: auto PushState(std::string state);
protected: auto PopState(bool drop);
protected: void Eat(TokenType tokenType);
protected: auto Backtrack();
protected: void Error(std::string msg);
protected: [[nodiscard]] bool IsAtEnd();
protected: [[nodiscard]] bool IsType(TokenType tokenType);
protected: [[nodiscard]] bool IsKeyword();
protected: [[nodiscard]] bool IsState(std::string state);
public: [[nodiscard]] ASTNode* Parse();
protected: [[nodiscard]] ASTNode* Expr();
protected: [[nodiscard]] ASTNode* AssignExpr();
protected: [[nodiscard]] ASTNode* CompExpr();
protected: [[nodiscard]] ASTNode* Term();
protected: [[nodiscard]] ASTNode* Factor();
protected: [[nodiscard]] ASTNode* Unary();
protected: [[nodiscard]] ASTNode* FunctionCall();
protected: [[nodiscard]] ASTNode* DotAccess();
protected: [[nodiscard]] ASTNode* IndexAccess();
protected: [[nodiscard]] ASTNode* Primary();
protected: [[nodiscard]] ASTNode* Variable();
protected: [[nodiscard]] ASTNode* ScopeResolution();
protected: [[nodiscard]] ASTNode* TemplateArgs(ASTNode* node);
protected: [[nodiscard]] std::vector<ASTNode*> ArgsValue();
protected: [[nodiscard]] ASTNode* Statement();
protected: [[nodiscard]] ASTNode* Body();
protected: [[nodiscard]] ASTNode* Type();
protected: [[nodiscard]] ASTNode* ArgDecl();
protected: [[nodiscard]] std::vector<ASTNode*> TypedArgs();
protected: [[nodiscard]] ASTNode* TemplateDecl();
protected: [[nodiscard]] ASTNode* FunctionDecl();
};
