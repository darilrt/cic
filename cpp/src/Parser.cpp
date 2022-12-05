#include "Parser.hpp"
#include "vector"
#include "Token.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
ParserState::ParserState(std::string state, int current) {
    this->state = state;
    this->current = current;
}
Parser::Parser(Lexer* lexer, std::vector<int> lines) {
    this->lexer = lexer;
    this->tokens = lexer->Parse();
    this->current = 0;
    this->currentToken = &this->tokens[this->current];
    while ((this->currentToken->tokenType) == TokenType::NewLine) {
        this->current += 1;
        this->currentToken = &this->tokens[this->current];
    }
    this->prevToken = 0;
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
void Parser::PrintLine(int line) {
    int lineStart = this->lines[line];
    while ((lineStart) < this->lexer->source.size()) {
        char const c = this->lexer->source[lineStart];
        if (c == '\n') {
            break;
        }
        std::cout << c;
        lineStart++;
    }
}
void Parser::Eat(TokenType tokenType) {
    if ((this->currentToken->tokenType) == tokenType) {
        this->current += 1;
        this->prevToken = this->currentToken;
        this->currentToken = &this->tokens[this->current];
        while ((this->currentToken->tokenType) == TokenType::NewLine) {
            this->current += 1;
            this->prevToken = this->currentToken;
            this->currentToken = &this->tokens[this->current];
        }
    } else {
        std::string error = "Expected token type: ";
        error += TokenTypeToString(tokenType);
        error += ", got: ";
        error += TokenTypeToString(this->currentToken->tokenType);
        error += " at line: ";
        error += std::to_string(this->currentToken->line);
        error += ", column: ";
        error += std::to_string(this->currentToken->column);
        this->Error(error);
        this->PrintLine(this->currentToken->line);
    }
}
void Parser::EatNewLine() {
    this->Eat(TokenType::Semicolon);
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
    while ((this->IsType(TokenType::Assign)) || (this->IsType(TokenType::AddAssign)) || (this->IsType(TokenType::SubAssign)) || (this->IsType(TokenType::MulAssign)) || (this->IsType(TokenType::DivAssign)) || (this->IsType(TokenType::ModAssign)) || (this->IsType(TokenType::AndAssign)) || (this->IsType(TokenType::OrAssign)) || (this->IsType(TokenType::XorAssign)) || (this->IsType(TokenType::LShiftAssign)) || (this->IsType(TokenType::RShiftAssign))) {
        Token* const operation = this->currentToken;
        this->Eat(this->currentToken->tokenType);
        ASTNode* const right = this->CompExpr();
        node = new ASTBinaryOp(node, *operation, right);
    }
    return node;
}
ASTNode* Parser::CompExpr() {
    ASTNode* node = this->ShiftExpr();
    if ((this->IsType(TokenType::Equal)) || (this->IsType(TokenType::NotEqual)) || (this->IsType(TokenType::Less)) || (this->IsType(TokenType::LessEqual)) || (this->IsType(TokenType::Greater)) || (this->IsType(TokenType::GreaterEqual))) {
        if ((this->IsInsideState("TemplateArgs")) && (this->IsType(TokenType::Greater))) {
            return node;
        }
        Token* const operation = this->currentToken;
        this->Eat(operation->tokenType);
        ASTNode* const right = this->Expr();
        node = new ASTBinaryOp(node, *operation, right);
    }
    return node;
}
ASTNode* Parser::ShiftExpr() {
    ASTNode* node = this->Term();
    if ((this->IsType(TokenType::LShift)) || (this->IsType(TokenType::RShift))) {
        Token* const operation = this->currentToken;
        this->Eat(operation->tokenType);
        ASTNode* const right = this->Expr();
        node = new ASTBinaryOp(node, *operation, right);
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
    ASTNode* node = this->IncDecExpr();
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
ASTNode* Parser::IncDecExpr() {
    ASTNode* node = this->Unary();
    if ((this->IsType(TokenType::Inc)) || (this->IsType(TokenType::Dec))) {
        Token* const token = this->currentToken;
        this->Eat(token->tokenType);
        ASTUnaryOp* const un = new ASTUnaryOp(*token, node);
        un->isPostfix = true;
        node = un;
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
    } else if (this->IsType(TokenType::New)) {
        node = this->New();
    } else if (this->IsType(TokenType::Del)) {
        node = this->Del();
    } else if (this->IsType(TokenType::String)) {
        node = new ASTConstant('"' + this->currentToken->literal + '"');
        this->Eat(TokenType::String);
    } else if (this->IsType(TokenType::Char)) {
        node = new ASTConstant("'" + this->currentToken->literal + "'");
        this->Eat(TokenType::Char);
    } else if (this->IsType(TokenType::Dot)) {
        this->Eat(TokenType::Dot);
        node = this->ScopeResolution();
        return new ASTThisAccess(node);
    } else {
        if (this->IsInsideState("CheckingForPointer")) {
            return 0;
        }
        this->Error("Expected an expression");
    }
    return node;
}
ASTNode* Parser::New() {
    this->Eat(TokenType::New);
    ASTNode* const node = this->Expr();
    return new ASTNew(node);
}
ASTNode* Parser::Del() {
    this->Eat(TokenType::Del);
    bool isArray = false;
    if (this->IsType(TokenType::LBracket)) {
        this->Eat(TokenType::LBracket);
        this->Eat(TokenType::RBracket);
        isArray = true;
    }
    ASTNode* const node = this->Expr();
    return new ASTDel(node, isArray);
}
ASTNode* Parser::IdentDecl() {
    ASTNode* node = new ASTVariable(this->currentToken->literal);
    this->Eat(TokenType::Ident);
    while (this->IsType(TokenType::LBracket)) {
        this->Eat(TokenType::LBracket);
        ASTNode* size = 0;
        if (!this->IsType(TokenType::RBracket)) {
            size = this->Expr();
        }
        this->Eat(TokenType::RBracket);
        node = new ASTIndexOp(node, size);
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
        } else if (bodyType == BodyType::Impl) {
            stmt = this->ImplStatement();
        } else if (bodyType == BodyType::Class) {
            stmt = this->ClassStatement();
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
    ASTType* _type = 0;
    if (this->IsType(TokenType::Arrow)) {
        this->Eat(TokenType::Arrow);
        _type = this->Type();
    }
    ASTBody* body = 0;
    if (this->IsType(TokenType::LBrace)) {
        body = this->Body(BodyType::Impl);
    }
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
ASTWhile* Parser::WhileStmt() {
    this->Eat(TokenType::While);
    ASTExpr* const cond = this->Expr();
    ASTBody* const body = this->Body(BodyType::Impl);
    return new ASTWhile(cond, body);
}
ASTImport* Parser::ImportStmt() {
    this->Eat(TokenType::Import);
    Token* const path = this->currentToken;
    this->Eat(TokenType::String);
    return new ASTImport(path);
}
ASTInherArg* Parser::InherArg() {
    Token* protection = 0;
    if (this->IsType(TokenType::Priv)) {
        protection = this->currentToken;
        this->Eat(TokenType::Priv);
    } else if (this->IsType(TokenType::Pub)) {
        protection = this->currentToken;
        this->Eat(TokenType::Pub);
    }
    ASTNode* const name = this->ScopeResolution();
    return new ASTInherArg(protection, name);
}
std::vector<ASTInherArg*> Parser::InherArgs() {
    std::vector<ASTInherArg*> args;
    args.push_back(this->InherArg());
    while (this->IsType(TokenType::Comma)) {
        this->Eat(TokenType::Comma);
        args.push_back(this->InherArg());
    }
    return args;
}
ASTClassDecl* Parser::ClassDecl() {
    this->Eat(TokenType::Class);
    ASTVariable* const name = new ASTVariable(this->currentToken->literal);
    this->Eat(TokenType::Ident);
    ASTTemplateDecl* tmp = 0;
    if (this->IsType(TokenType::Less)) {
        tmp = this->TemplateDecl();
    }
    std::vector<ASTInherArg*> args;
    if (this->IsType(TokenType::LParen)) {
        this->Eat(TokenType::LParen);
        args = this->InherArgs();
        this->Eat(TokenType::RParen);
    }
    ASTBody* const body = this->Body(BodyType::Class);
    return new ASTClassDecl(name, tmp, args, body);
}
std::string Parser::Operator() {
    std::string const operation = "";
    if (this->IsType(TokenType::Add)) {
        this->Eat(TokenType::Add);
        return "+";
    } else if (this->IsType(TokenType::Sub)) {
        this->Eat(TokenType::Sub);
        return "-";
    } else if (this->IsType(TokenType::Mul)) {
        this->Eat(TokenType::Mul);
        return "*";
    } else if (this->IsType(TokenType::Div)) {
        this->Eat(TokenType::Div);
        return "/";
    } else if (this->IsType(TokenType::Mod)) {
        this->Eat(TokenType::Mod);
        return "%";
    } else if (this->IsType(TokenType::Xor)) {
        this->Eat(TokenType::Xor);
        return "^";
    } else if (this->IsType(TokenType::Amp)) {
        this->Eat(TokenType::Amp);
        return "&";
    } else if (this->IsType(TokenType::Pipe)) {
        this->Eat(TokenType::Pipe);
        return "|";
    } else if (this->IsType(TokenType::BitNot)) {
        this->Eat(TokenType::BitNot);
        return "~";
    } else if (this->IsType(TokenType::Not)) {
        this->Eat(TokenType::Not);
        return "!";
    } else if (this->IsType(TokenType::Assign)) {
        this->Eat(TokenType::Assign);
        return "=";
    } else if (this->IsType(TokenType::Less)) {
        this->Eat(TokenType::Less);
        return "<";
    } else if (this->IsType(TokenType::Greater)) {
        this->Eat(TokenType::Greater);
        return ">";
    } else if (this->IsType(TokenType::AddAssign)) {
        this->Eat(TokenType::AddAssign);
        return "+=";
    } else if (this->IsType(TokenType::SubAssign)) {
        this->Eat(TokenType::SubAssign);
        return "-=";
    } else if (this->IsType(TokenType::MulAssign)) {
        this->Eat(TokenType::MulAssign);
        return "*=";
    } else if (this->IsType(TokenType::DivAssign)) {
        this->Eat(TokenType::DivAssign);
        return "/=";
    } else if (this->IsType(TokenType::ModAssign)) {
        this->Eat(TokenType::ModAssign);
        return "%=";
    } else if (this->IsType(TokenType::XorAssign)) {
        this->Eat(TokenType::XorAssign);
        return "^=";
    } else if (this->IsType(TokenType::AndAssign)) {
        this->Eat(TokenType::AndAssign);
        return "&=";
    } else if (this->IsType(TokenType::OrAssign)) {
        this->Eat(TokenType::OrAssign);
        return "|=";
    } else if (this->IsType(TokenType::LShift)) {
        this->Eat(TokenType::LShift);
        return "<<";
    } else if (this->IsType(TokenType::RShift)) {
        this->Eat(TokenType::RShift);
        return ">>";
    } else if (this->IsType(TokenType::RShiftAssign)) {
        this->Eat(TokenType::RShiftAssign);
        return ">>=";
    } else if (this->IsType(TokenType::LShiftAssign)) {
        this->Eat(TokenType::LShiftAssign);
        return "<<=";
    } else if (this->IsType(TokenType::Equal)) {
        this->Eat(TokenType::Equal);
        return "==";
    } else if (this->IsType(TokenType::NotEqual)) {
        this->Eat(TokenType::NotEqual);
        return "!=";
    } else if (this->IsType(TokenType::LessEqual)) {
        this->Eat(TokenType::LessEqual);
        return "<=";
    } else if (this->IsType(TokenType::GreaterEqual)) {
        this->Eat(TokenType::GreaterEqual);
        return ">=";
    } else if (this->IsType(TokenType::And)) {
        this->Eat(TokenType::And);
        return "&&";
    } else if (this->IsType(TokenType::Or)) {
        this->Eat(TokenType::Or);
        return "||";
    } else if (this->IsType(TokenType::Inc)) {
        this->Eat(TokenType::Inc);
        return "++";
    } else if (this->IsType(TokenType::Dec)) {
        this->Eat(TokenType::Dec);
        return "--";
    } else if (this->IsType(TokenType::Arrow)) {
        this->Eat(TokenType::Arrow);
        return "->";
    } else if (this->IsType(TokenType::LBrace)) {
        this->Eat(TokenType::LBrace);
        this->Eat(TokenType::RBrace);
        return "[]";
    } else if (this->IsType(TokenType::LParen)) {
        this->Eat(TokenType::LParen);
        this->Eat(TokenType::RParen);
        return "()";
    } else {
        this->Error("Expected operator");
    }
    return operation;
}
ASTOperatorDecl* Parser::OperatorDecl() {
    this->Eat(TokenType::Operator);
    std::string const operation = this->Operator();
    this->Eat(TokenType::LParen);
    std::vector<ASTArgDecl*> const args = this->TypedArgs();
    this->Eat(TokenType::RParen);
    this->Eat(TokenType::Arrow);
    ASTType* const _type = this->Type();
    ASTBody* const body = this->Body(BodyType::Impl);
    return new ASTOperatorDecl(operation, args, _type, body);
}
ASTEnumField* Parser::EnumField() {
    ASTNode* const name = new ASTVariable(this->currentToken->literal);
    this->Eat(TokenType::Ident);
    ASTExpr* value = 0;
    if (this->IsType(TokenType::Assign)) {
        this->Eat(TokenType::Assign);
        value = this->Expr();
    }
    return new ASTEnumField(name, value);
}
std::vector<ASTEnumField*> Parser::EnumFields() {
    std::vector<ASTEnumField*> fields;
    while (!this->IsType(TokenType::RBrace)) {
        fields.push_back(this->EnumField());
        if (this->IsType(TokenType::Comma)) {
            this->Eat(TokenType::Comma);
        }
    }
    return fields;
}
ASTEnumDecl* Parser::EnumDecl() {
    this->Eat(TokenType::Enum);
    ASTNode* const name = new ASTVariable(this->currentToken->literal);
    this->Eat(TokenType::Ident);
    this->Eat(TokenType::LBrace);
    std::vector<ASTEnumField*> const fields = this->EnumFields();
    this->Eat(TokenType::RBrace);
    return new ASTEnumDecl(name, fields);
}
ASTNode* Parser::DeclStatement() {
    if (this->IsType(TokenType::Semicolon)) {
        this->Eat(TokenType::Semicolon);
        return 0;
    } else if ((this->IsType(TokenType::Let)) || (this->IsType(TokenType::Mut))) {
        ASTNode* const node = this->VariableDecl();
        this->EatNewLine();
        return node;
    } else if (this->IsType(TokenType::Ident)) {
        return this->FunctionDecl();
    } else if (this->IsType(TokenType::Import)) {
        return this->ImportStmt();
    } else if (this->IsType(TokenType::Class)) {
        return this->ClassDecl();
    } else if (this->IsType(TokenType::Enum)) {
        return this->EnumDecl();
    } else {
        this->Error("Expected declaration statement");
    }
    return 0;
}
ASTNode* Parser::ImplStatement() {
    ASTNode* node = 0;
    if (this->IsType(TokenType::Semicolon)) {
        return 0;
    } else if ((this->IsType(TokenType::Let)) || (this->IsType(TokenType::Mut))) {
        node = this->VariableDecl();
        this->EatNewLine();
    } else if (this->IsType(TokenType::Ret)) {
        node = this->ReturnStmt();
        this->EatNewLine();
    } else if (this->IsType(TokenType::If)) {
        node = this->IfStmt();
    } else if (this->IsType(TokenType::While)) {
        node = this->WhileStmt();
    } else if (this->IsType(TokenType::Break)) {
        this->Eat(TokenType::Break);
        node = new ASTBreak();
        this->EatNewLine();
    } else if (this->IsType(TokenType::Continue)) {
        this->Eat(TokenType::Continue);
        node = new ASTContinue();
        this->EatNewLine();
    } else {
        node = this->Expr();
        this->EatNewLine();
    }
    return node;
}
ASTNode* Parser::ClassStatement() {
    ASTNode* node = 0;
    if (this->IsType(TokenType::Pub)) {
        this->Eat(TokenType::Pub);
        node = this->ClassStatement();
        node->attr |= Attribute::Public;
    } else if (this->IsType(TokenType::Priv)) {
        this->Eat(TokenType::Priv);
        node = this->ClassStatement();
        node->attr |= Attribute::Private;
    } else if (this->IsType(TokenType::Virtual)) {
        this->Eat(TokenType::Virtual);
        node = this->ClassStatement();
        node->attr |= Attribute::Virtual;
    } else if (this->IsType(TokenType::Static)) {
        this->Eat(TokenType::Static);
        node = this->ClassStatement();
        node->attr |= Attribute::Static;
    } else if (this->IsType(TokenType::Operator)) {
        node = this->OperatorDecl();
    } else if ((this->IsType(TokenType::Let)) || (this->IsType(TokenType::Mut))) {
        node = this->VariableDecl();
        this->EatNewLine();
    } else if (this->IsType(TokenType::Ident)) {
        node = this->FunctionDecl();
    } else if (this->IsType(TokenType::Xor)) {
        this->Eat(TokenType::Xor);
        node = this->FunctionDecl();
        node->attr |= Attribute::Destructor;
    } else {
        this->Error("Expected class statement");
    }
    return node;
}
