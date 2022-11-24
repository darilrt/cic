#include "Parser.hpp"
#include "vector"
#include "Token.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
ParserState::ParserState(std::string state, int current) {
    this->state = state;
    this->current = current;
}
Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
    this->tokens = lexer->Parse();
    this->current = 0;
    this->currentToken = &this->tokens[this->current];
}
auto Parser::PushState(std::string state) {
    this->state = state;
    this->stateStack.push_back(ParserState(this->state, this->current));
}
auto Parser::PopState(bool drop) {
    if ((this->stateStack.size()) > 0) {
        this->state = (this->stateStack.back()).state;
        if (!drop) {
            this->current = (this->stateStack.back()).current;
            this->currentToken = &this->tokens[this->current];
        }
    }
    this->stateStack.pop_back();
}
void Parser::Eat(TokenType tokenType) {
    if ((this->currentToken->tokenType) == tokenType) {
        this->current += 1;
        this->currentToken = &this->tokens[this->current];
    } else {
        this->Error("Unexpected " + (this->currentToken->ToString()));
    }
}
auto Parser::Backtrack() {
    if (this->current > 0) {
        this->current -= 1;
        this->currentToken = &this->tokens[this->current];
    }
}
void Parser::Error(std::string msg) {
    std::cout << "Error: " << msg << std::endl;
}
bool Parser::IsAtEnd() {
    return (this->currentToken->tokenType) == (TokenType::EndOfFile);
}
bool Parser::IsType(TokenType tokenType) {
    return (this->currentToken->tokenType) == tokenType;
}
bool Parser::IsKeyword() {
    return (this->currentToken->tokenType) > (TokenType::StartKeyword) && (this->currentToken->tokenType) < (TokenType::EndKeyword);
}
bool Parser::IsState(std::string state) {
    return this->state == state;
}
ASTNode* Parser::Parse() {
    return this->Statement();
}
ASTNode* Parser::Expr() {
    return this->AssignExpr();
}
ASTNode* Parser::AssignExpr() {
    ASTNode* node = this->CompExpr();
    while ((this->IsType(TokenType::Assign)) || (this->IsType(TokenType::AddAssign)) || (this->IsType(TokenType::SubAssign)) || (this->IsType(TokenType::MulAssign)) || (this->IsType(TokenType::DivAssign)) || (this->IsType(TokenType::ModAssign))) {
        Token* const op = this->currentToken;
        this->Eat(this->currentToken->tokenType);
        ASTNode* const right = this->CompExpr();
        node = new ASTBinaryOp(node, *op, right);
    }
    return node;
}
ASTNode* Parser::CompExpr() {
    ASTNode* node = this->Term();
    if ((this->IsType(TokenType::Equal)) || (this->IsType(TokenType::NotEqual)) || (this->IsType(TokenType::Less)) || (this->IsType(TokenType::LessEqual)) || (this->IsType(TokenType::Greater)) || (this->IsType(TokenType::GreaterEqual))) {
        if (this->state == "TemplateArgs" && (this->IsType(TokenType::Greater))) {
            return node;
        }
        Token* const op = this->currentToken;
        this->Eat(op->tokenType);
        ASTNode* const right = this->Expr();
        node = new ASTBinaryOp(node, *op, right);
    }
    return node;
}
ASTNode* Parser::Term() {
    ASTNode* node = this->Factor();
    if ((this->IsType(TokenType::Add)) || (this->IsType(TokenType::Sub))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const right = this->Expr();
        node = new ASTBinaryOp(node, *token, right);
    }
    return node;
}
ASTNode* Parser::Factor() {
    ASTNode* node = this->Unary();
    if ((this->IsType(TokenType::Mul)) || (this->IsType(TokenType::Div)) || (this->IsType(TokenType::Mod))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const right = Factor();
        node = new ASTBinaryOp(node, *token, right);
    }
    return node;
}
ASTNode* Parser::Unary() {
    if ((this->IsType(TokenType::Add)) || (this->IsType(TokenType::Sub)) || (this->IsType(TokenType::Not)) || (this->IsType(TokenType::BitNot))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const right = this->Unary();
        return new ASTUnaryOp(*token, right);
    } else if ((this->IsType(TokenType::And)) || (this->IsType(TokenType::Mul))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const right = this->Unary();
        return new ASTUnaryOp(*token, right);
    }
    return this->FunctionCall();
}
ASTNode* Parser::FunctionCall() {
    ASTNode* node = this->DotAccess();
    if (this->IsType(TokenType::LParen)) {
        this->Eat(TokenType::LParen);
        if (this->IsType(TokenType::RParen)) {
            this->Eat(TokenType::RParen);
            return new ASTFunctionCall(node);
        }
        std::vector<ASTNode*> const args = this->ArgsValue();
        this->Eat(TokenType::RParen);
        return new ASTFunctionCall(node, args);
    }
    return node;
}
ASTNode* Parser::DotAccess() {
    ASTNode* node = this->IndexAccess();
    while ((this->IsType(TokenType::Dot)) || (this->IsType(TokenType::Arrow))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const right = this->Primary();
        node = new ASTDotAccess(node, *token, right);
    }
    return node;
}
ASTNode* Parser::IndexAccess() {
    ASTNode* node = this->Primary();
    while (this->IsType(TokenType::LBracket)) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const index = this->Expr();
        this->Eat(TokenType::RBracket);
        node = new ASTIndexOp(node, index);
    }
    return node;
}
ASTNode* Parser::Primary() {
    ASTNode* node;
    if (this->IsType(TokenType::Int)) {
        node = new ASTConstant(this->currentToken->literal);
        this->Eat(TokenType::Int);
    } else if (this->IsType(TokenType::Float)) {
        node = new ASTConstant(this->currentToken->literal);
        this->Eat(TokenType::Float);
    } else if (this->IsType(TokenType::True)) {
        node = new ASTConstant(this->currentToken->literal);
        this->Eat(TokenType::True);
    } else if (this->IsType(TokenType::Null)) {
        node = new ASTConstant(this->currentToken->literal);
        this->Eat(TokenType::Null);
    } else if (this->IsType(TokenType::False)) {
        node = new ASTConstant(this->currentToken->literal);
        this->Eat(TokenType::False);
    } else if (this->IsType(TokenType::LParen)) {
        this->Eat(TokenType::LParen);
        node = this->Expr();
        this->Eat(TokenType::RParen);
    } else if (this->IsType(TokenType::Ident)) {
        node = this->ScopeResolution();
    } else {
        this->Error("Expected an expression");
    }
    return node;
}
ASTNode* Parser::Variable() {
    ASTNode* node;
    if (this->IsType(TokenType::Ident)) {
        node = new ASTVariable(this->currentToken->literal);
        this->Eat(TokenType::Ident);
    } else if (this->IsType(TokenType::Class)) {
        node = new ASTVariable(this->currentToken->literal);
        this->Eat(TokenType::Class);
    } else if (this->IsType(TokenType::Type)) {
        node = new ASTVariable(this->currentToken->literal);
        this->Eat(TokenType::Type);
    } else {
        this->Error("Expected an identifier");
    }
    if (this->IsType(TokenType::Less)) {
        ASTNode* const tmp = this->TemplateArgs(node);
        if (tmp != 0) {
            node = tmp;
        }
    }
    return node;
}
ASTNode* Parser::ScopeResolution() {
    ASTNode* node = this->Variable();
    while (this->IsType(TokenType::ScopeResolution)) {
        this->Eat(TokenType::ScopeResolution);
        node = new ASTScopeResolution(node, this->Variable());
    }
    while (this->IsType(TokenType::Mul)) {
        this->Eat(TokenType::Mul);
        node = new ASTPointerDecl(node);
    }
    if (this->IsType(TokenType::And)) {
        this->Eat(TokenType::And);
        node = new ASTReferenceDecl(node);
    }
    return node;
}
ASTNode* Parser::TemplateArgs(ASTNode* node) {
    this->PushState("TemplateArgs");
    this->Eat(TokenType::Less);
    std::vector<ASTNode*> const args = this->ArgsValue();
    if (this->IsType(TokenType::Greater)) {
        this->Eat(TokenType::Greater);
        this->PopState(true);
        return new ASTTemplateArgs(node, args);
    }
    this->PopState(false);
    int i = 0;
    while ((i) < args.size()) {
        delete args[i];
        i += 1;
    }
    return 0;
}
std::vector<ASTNode*> Parser::ArgsValue() {
    std::vector<ASTNode*> args;
    args.push_back(this->Expr());
    while (this->IsType(TokenType::Comma)) {
        this->Eat(TokenType::Comma);
        args.push_back(this->Expr());
    }
    return args;
}
ASTNode* Parser::Statement() {
    ASTNode* const node = this->Expr();
    return node;
}
ASTNode* Parser::Body() {
    this->Eat(TokenType::LBrace);
    ASTBody body = ASTBody();
    while (!this->IsType(TokenType::RBrace)) {
        body.AddStatement(this->Statement());
    }
    this->Eat(TokenType::RBrace);
    return new ASTBody(body);
}
ASTNode* Parser::Type() {
    ASTNode* const left = this->Variable();
    return new ASTType(left);
}
ASTNode* Parser::ArgDecl() {
    ASTNode* const name = new ASTVariable(this->currentToken->literal);
    this->Eat(TokenType::Colon);
    ASTNode* const _type = this->Type();
    return new ASTArgDecl(name, _type);
}
std::vector<ASTNode*> Parser::TypedArgs() {
    std::vector<ASTNode*> args;
    args.push_back(this->ArgDecl());
    while (this->IsType(TokenType::Comma)) {
        this->Eat(TokenType::Comma);
        args.push_back(this->ArgDecl());
    }
    return args;
}
ASTNode* Parser::TemplateDecl() {
    this->PushState("TemplateDecl");
    this->Eat(TokenType::Less);
    std::vector<ASTNode*> const args = this->TypedArgs();
    if (this->IsType(TokenType::Greater)) {
        this->Eat(TokenType::Greater);
        this->PopState(true);
        return new ASTTemplateDecl(args);
    }
    this->PopState(false);
    int i = 0;
    while ((i) < args.size()) {
        delete args[i];
        i += 1;
    }
    return 0;
}
ASTNode* Parser::FunctionDecl() {
    ASTNode* const name = new ASTVariable(this->currentToken->literal);
    this->Eat(TokenType::Ident);
    ASTNode* templateDecl = 0;
    if (this->IsType(TokenType::Less)) {
        templateDecl = this->TemplateDecl();
    }
    this->Eat(TokenType::LParen);
    std::vector<ASTNode*> const args = this->TypedArgs();
    this->Eat(TokenType::RParen);
    this->Eat(TokenType::Colon);
    ASTNode* const _type = this->Type();
    ASTNode* const body = this->Body();
    return new ASTFunctionDecl(name, templateDecl, args, _type, body);
}
