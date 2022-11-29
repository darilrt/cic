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
ASTNode* Parser::Parse() {
    return this->Program();
}
void Parser::PushState(std::string state) {
    this->state = state;
    this->stateStack.push_back(ParserState(this->state, this->current));
}
void Parser::PopState(bool drop) {
    if ((this->stateStack.size()) > 0) {
        this->state = (this->stateStack.back()).state;
        if (!drop) {
            this->current = (this->stateStack.back()).current;
            this->currentToken = &this->tokens[this->current];
        }
    }
    this->stateStack.pop_back();
}
void Parser::ShowState() {
    std::cout << "State: ";
    int i = 0;
    while ((i) < this->stateStack.size()) {
        std::cout << (this->stateStack[i]).state << " <- ";
        i++;
    }
    std::cout << "\n";
}
bool Parser::IsInsideState(std::string state) {
    int i = 0;
    while ((i) < this->stateStack.size()) {
        if ((this->stateStack[i]).state == state) {
            return true;
        }
        i++;
    }
    return false;
}
void Parser::Eat(TokenType tokenType) {
    if ((this->currentToken->tokenType) == tokenType) {
        this->current += 1;
        this->currentToken = &this->tokens[this->current];
    } else {
        this->Error("Expected " + TokenTypeToString(tokenType) + ", got " + TokenTypeToString(this->currentToken->tokenType));
    }
}
void Parser::Backtrack() {
    if (this->current > 0) {
        this->current -= 1;
        this->currentToken = &this->tokens[this->current];
    }
}
void Parser::Error(std::string msg) {
    std::cout << "Error: " << msg << std::endl;
    exit(0);
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
ASTNode* Parser::Program() {
    std::vector<ASTNode*> decls;
    while (!this->IsAtEnd()) {
        decls.push_back(this->DeclStatement());
    }
    return new ASTProgram(decls);
}
ASTExpr* Parser::Expr() {
    return new ASTExpr(this->AssignExpr());
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
    ASTNode* node = this->ShiftExpr();
    if ((this->IsType(TokenType::Equal)) || (this->IsType(TokenType::NotEqual)) || (this->IsType(TokenType::Less)) || (this->IsType(TokenType::LessEqual)) || (this->IsType(TokenType::Greater)) || (this->IsType(TokenType::GreaterEqual))) {
        if ((this->IsInsideState("TemplateArgs")) && (this->IsType(TokenType::Greater))) {
            return node;
        }
        Token* const op = this->currentToken;
        this->Eat(op->tokenType);
        ASTNode* const right = this->Expr();
        node = new ASTBinaryOp(node, *op, right);
    }
    return node;
}
ASTNode* Parser::ShiftExpr() {
    ASTNode* node = this->Term();
    if ((this->IsType(TokenType::LShift)) || (this->IsType(TokenType::RShift))) {
        Token* const op = this->currentToken;
        this->Eat(op->tokenType);
        ASTNode* const right = this->Expr();
        node = new ASTBinaryOp(node, *op, right);
    }
    return node;
}
ASTNode* Parser::Term() {
    ASTNode* node = this->BitwiseExpr();
    if ((this->IsType(TokenType::Add)) || (this->IsType(TokenType::Sub))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const right = this->Expr();
        node = new ASTBinaryOp(node, *token, right);
    }
    return node;
}
ASTNode* Parser::BitwiseExpr() {
    ASTNode* node = this->Factor();
    if ((this->IsType(TokenType::And))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        this->PushState("CheckingForPointer");
        ASTNode* const right = this->Expr();
        if (right != 0) {
            this->PopState(true);
            node = new ASTBinaryOp(node, *token, right);
        } else {
            this->PopState(false);
            node = new ASTReferenceDecl(node);
        }
    } else if ((this->IsType(TokenType::Or)) || (this->IsType(TokenType::Xor))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const right = this->Expr();
        node = new ASTBinaryOp(node, *token, right);
    }
    return node;
}
ASTNode* Parser::Factor() {
    ASTNode* node = this->Unary();
    if (this->IsType(TokenType::Mul)) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        this->PushState("CheckingForPointer");
        ASTNode* const right = this->Factor();
        if (right != 0) {
            this->PopState(true);
            node = new ASTBinaryOp(node, *token, right);
        } else {
            this->PopState(false);
            node = new ASTPointerDecl(node);
        }
    } else if ((this->IsType(TokenType::Div)) || (this->IsType(TokenType::Mod))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTNode* const right = this->Factor();
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
    return this->DotAccess();
}
ASTNode* Parser::FunctionCall() {
    ASTNode* node = this->IndexAccess(this->Primary());
    if (node == 0) {
        return 0;
    }
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
    ASTNode* node = this->IndexAccess(this->FunctionCall());
    if (node == 0) {
        return 0;
    }
    if ((this->IsType(TokenType::Dot)) || (this->IsType(TokenType::Arrow))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        node = new ASTDotAccess(node, *token, this->DotAccess());
    }
    return node;
}
ASTNode* Parser::IndexAccess(ASTNode* node) {
    if (node == 0) {
        return 0;
    }
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
    } else if (this->IsType(TokenType::String)) {
        node = new ASTConstant(this->currentToken->literal);
        this->Eat(TokenType::String);
    } else {
        if (this->IsInsideState("CheckingForPointer")) {
            return 0;
        }
        this->Error("Expected an expression");
    }
    return node;
}
ASTNode* Parser::IdentDecl() {
    ASTNode* node = new ASTVariable(this->currentToken->literal);
    this->Eat(TokenType::Ident);
    while (this->IsType(TokenType::LBracket)) {
        this->Eat(TokenType::LBracket);
        ASTNode* const index = this->Expr();
        this->Eat(TokenType::RBracket);
        node = new ASTIndexOp(node, index);
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
        ASTTemplateArgs* const tmp = this->TemplateArgs(node);
        if (tmp != 0) {
            node = tmp;
        }
    }
    return node;
}
ASTNode* Parser::ScopeResolution() {
    ASTNode* node = this->Variable();
    if (this->IsType(TokenType::Less)) {
        ASTTemplateArgs* const tmp = this->TemplateArgs(node);
        if (tmp != 0) {
            node = tmp;
        }
    }
    while (this->IsType(TokenType::ScopeResolution)) {
        this->Eat(TokenType::ScopeResolution);
        ASTNode* right = this->Variable();
        if (this->IsType(TokenType::Less)) {
            ASTTemplateArgs* const tmp = this->TemplateArgs(right);
            if (tmp != 0) {
                right = tmp;
            }
        }
        node = new ASTScopeResolution(node, right);
    }
    return node;
}
ASTTemplateArgs* Parser::TemplateArgs(ASTNode* node) {
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
ASTBody* Parser::Body(BodyType bodyType) {
    this->Eat(TokenType::LBrace);
    ASTBody* body = new ASTBody();
    ASTNode* stmt;
    while (!(this->IsType(TokenType::RBrace))) {
        if (bodyType == BodyType::Decl) {
            stmt = this->DeclStatement();
        } else {
            stmt = this->ImplStatement();
        }
        body->AddStatement(stmt);
        if (this->IsType(TokenType::EndOfFile)) {
            break;
        }
    }
    this->Eat(TokenType::RBrace);
    return body;
}
ASTType* Parser::Type() {
    bool isConst = false;
    if (this->IsType(TokenType::Const)) {
        this->Eat(TokenType::Const);
        isConst = true;
    }
    ASTNode* left = this->ScopeResolution();
    while (this->IsType(TokenType::Mul)) {
        this->Eat(TokenType::Mul);
        left = new ASTPointerDecl(left);
    }
    if (this->IsType(TokenType::And)) {
        this->Eat(TokenType::And);
        left = new ASTReferenceDecl(left);
    }
    ASTType* ntype = new ASTType(left);
    ntype->isConst = isConst;
    return ntype;
}
ASTArgDecl* Parser::ArgDecl() {
    ASTNode* const name = this->IdentDecl();
    this->Eat(TokenType::Colon);
    ASTNode* const _type = this->Type();
    return new ASTArgDecl(name, _type);
}
std::vector<ASTArgDecl*> Parser::TypedArgs() {
    std::vector<ASTArgDecl*> args;
    args.push_back(this->ArgDecl());
    while (this->IsType(TokenType::Comma)) {
        this->Eat(TokenType::Comma);
        args.push_back(this->ArgDecl());
    }
    return args;
}
ASTTemplateDecl* Parser::TemplateDecl() {
    this->PushState("TemplateDecl");
    this->Eat(TokenType::Less);
    std::vector<ASTArgDecl*> const args = this->TypedArgs();
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
ASTVariableDecl* Parser::VariableDecl() {
    bool isMutable = false;
    if (this->IsType(TokenType::Mut)) {
        isMutable = true;
        this->Eat(TokenType::Mut);
    } else {
        this->Eat(TokenType::Let);
    }
    ASTNode* const name = this->IdentDecl();
    this->Eat(TokenType::Colon);
    ASTType* const _type = this->Type();
    ASTExpr* expr = 0;
    if (this->IsType(TokenType::Assign)) {
        this->Eat(TokenType::Assign);
        expr = this->Expr();
    }
    return new ASTVariableDecl(name, _type, expr, isMutable);
}
ASTFunctionDecl* Parser::FunctionDecl() {
    ASTVariable* const name = new ASTVariable(this->currentToken->literal);
    this->Eat(TokenType::Ident);
    ASTTemplateDecl* tmp = 0;
    if (this->IsType(TokenType::Less)) {
        tmp = this->TemplateDecl();
    }
    this->Eat(TokenType::LParen);
    std::vector<ASTArgDecl*> args;
    if (this->IsType(TokenType::RParen)) {
        this->Eat(TokenType::RParen);
    } else {
        args = this->TypedArgs();
        this->Eat(TokenType::RParen);
    }
    int attrs = false;
    if (this->IsType(TokenType::Const)) {
        this->Eat(TokenType::Const);
        attrs |= Attribute::Const;
    }
    this->Eat(TokenType::Arrow);
    ASTType* const _type = this->Type();
    ASTBody* const body = this->Body(BodyType::Impl);
    ASTFunctionDecl* func = new ASTFunctionDecl(name, tmp, args, _type, body);
    func->attr = attrs;
    return func;
}
ASTReturn* Parser::ReturnStmt() {
    this->Eat(TokenType::Ret);
    return new ASTReturn(this->Expr());
}
ASTIf* Parser::IfStmt() {
    this->Eat(TokenType::If);
    ASTExpr* const cond = this->Expr();
    ASTBody* const body = this->Body(BodyType::Impl);
    std::vector<ASTIf*> elifs;
    while (this->IsType(TokenType::Elif)) {
        this->Eat(TokenType::Elif);
        ASTExpr* const elifCond = this->Expr();
        ASTBody* const elifBody = this->Body(BodyType::Impl);
        elifs.push_back(new ASTIf(elifCond, elifBody));
    }
    ASTBody* elseBody = 0;
    if (this->IsType(TokenType::Else)) {
        this->Eat(TokenType::Else);
        elseBody = this->Body(BodyType::Impl);
    }
    return new ASTIf(cond, body, elifs, elseBody);
}
ASTNode* Parser::DeclStatement() {
    if (this->IsType(TokenType::Semicolon)) {
        this->Eat(TokenType::Semicolon);
        return 0;
    } else if ((this->IsType(TokenType::Let)) || (this->IsType(TokenType::Mut))) {
        ASTNode* const node = this->VariableDecl();
        this->Eat(TokenType::Semicolon);
        return node;
    } else if (this->IsType(TokenType::Ident)) {
        return this->FunctionDecl();
    }
    return 0;
}
ASTNode* Parser::ImplStatement() {
    ASTNode* node = 0;
    if (this->IsType(TokenType::Semicolon)) {
        this->Eat(TokenType::Semicolon);
        return 0;
    } else if ((this->IsType(TokenType::Let)) || (this->IsType(TokenType::Mut))) {
        node = this->VariableDecl();
        this->Eat(TokenType::Semicolon);
    } else if (this->IsType(TokenType::Ret)) {
        node = this->ReturnStmt();
        this->Eat(TokenType::Semicolon);
    } else if (this->IsType(TokenType::If)) {
        node = this->IfStmt();
    } else {
        node = this->Expr();
        this->Eat(TokenType::Semicolon);
    }
    return node;
}
