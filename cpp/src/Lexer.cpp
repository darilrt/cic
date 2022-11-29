#include "Lexer.hpp"
#include "iostream"
#include "vector"
#include "string"
#include "Token.hpp"
Lexer::Lexer(const std::string& source) {
    this->source = source;
    this->index = 0;
    this->column = 1;
    this->line = 1;
    this->currentChar = this->source[this->index];
}
char Lexer::Peek() {
    if ((this->index + 1) < this->source.size()) {
        return this->source[this->index + 1];
    }
    return '\0';
}
char Lexer::Peek2() {
    if ((this->index + 2) < this->source.size()) {
        return this->source[this->index + 2];
    }
    return '\0';
}
bool Lexer::PeekEquals(char expected) {
    char const peek = this->Peek();
    if (peek == expected) {
        return true;
    }
    return false;
}
bool Lexer::Peek2Equals(char expected) {
    char const peek = this->Peek2();
    if (peek == expected) {
        return true;
    }
    return false;
}
auto Lexer::Advance() {
    this->index += 1;
    this->column += 1;
    if (this->index >= this->source.size()) {
        this->currentChar = '\0';
    } else {
        this->currentChar = this->source[this->index];
    }
    if (this->IsNewline()) {
        this->column = 1;
        this->line += 1;
    }
}
auto Lexer::SkipWhitespace() {
    while (this->IsWhitespace()) {
        this->Advance();
    }
}
std::vector<Token> Lexer::Parse() {
    std::vector<Token> tokens;
    Token token;
    while (token.tokenType != TokenType::EndOfFile) {
        token = this->ParseToken();
        tokens.push_back(token);
    }
    return tokens;
}
Token Lexer::ParseToken() {
    while (this->IsNotNull()) {
        if (this->IsWhitespace()) {
            this->SkipWhitespace();
            continue;
        }
        if (this->IsNewline()) {
            this->Advance();
            return Token(TokenType::NewLine, "\n", this->line, this->column);
        }
        if (this->IsAlpha()) {
            return this->ParseIdentifier();
        }
        if (this->IsDigit()) {
            return this->ParseNumber();
        }
        if (this->IsChar('"')) {
            return this->ParseString();
        }
        if (this->IsChar(39)) {
            return this->ParseChar();
        }
        if (this->IsChar('/')) {
            if (this->PeekEquals('/')) {
                this->SkipComment();
                continue;
            }
        }
        if (this->Switch3('.', '.', '.')) {
            return Token(TokenType::Ellipsis, "...");
        }
        if (this->Switch3('<', '<', '=')) {
            return Token(TokenType::LShiftAssign, "<<=");
        }
        if (this->Switch3('>', '>', '=')) {
            return Token(TokenType::RShiftAssign, ">>=");
        }
        if (this->Switch2('=', '=')) {
            return Token(TokenType::Equal, "==");
        }
        if (this->Switch2('!', '=')) {
            return Token(TokenType::NotEqual, "!=");
        }
        if (this->Switch2('<', '=')) {
            return Token(TokenType::LessEqual, "<=");
        }
        if (this->Switch2('>', '=')) {
            return Token(TokenType::GreaterEqual, ">=");
        }
        if (this->Switch2('+', '+')) {
            return Token(TokenType::Inc, "++");
        }
        if (this->Switch2('-', '-')) {
            return Token(TokenType::Dec, "--");
        }
        if (this->Switch2('&', '&')) {
            return Token(TokenType::And, "&&");
        }
        if (this->Switch2('|', '|')) {
            return Token(TokenType::Or, "||");
        }
        if (this->Switch2('+', '=')) {
            return Token(TokenType::AddAssign, "+=");
        }
        if (this->Switch2('-', '=')) {
            return Token(TokenType::SubAssign, "-=");
        }
        if (this->Switch2('*', '=')) {
            return Token(TokenType::MulAssign, "*=");
        }
        if (this->Switch2('/', '=')) {
            return Token(TokenType::DivAssign, "/=");
        }
        if (this->Switch2('%', '=')) {
            return Token(TokenType::ModAssign, "%=");
        }
        if (this->Switch2('&', '=')) {
            return Token(TokenType::AndAssign, "&=");
        }
        if (this->Switch2('|', '=')) {
            return Token(TokenType::OrAssign, "|=");
        }
        if (this->Switch2('^', '=')) {
            return Token(TokenType::XorAssign, "^=");
        }
        if (this->Switch2('>', '>')) {
            return Token(TokenType::RShift, ">>");
        }
        if (this->Switch2('<', '<')) {
            return Token(TokenType::LShift, "<<");
        }
        if (this->Switch2('-', '>')) {
            return Token(TokenType::Arrow, "->");
        }
        if (this->Switch2(':', ':')) {
            return Token(TokenType::ScopeResolution, "::");
        }
        if (this->Switch1('+')) {
            return Token(TokenType::Add, "+");
        }
        if (this->Switch1('-')) {
            return Token(TokenType::Sub, "-");
        }
        if (this->Switch1('*')) {
            return Token(TokenType::Mul, "*");
        }
        if (this->Switch1('/')) {
            return Token(TokenType::Div, "/");
        }
        if (this->Switch1('%')) {
            return Token(TokenType::Mod, "%");
        }
        if (this->Switch1('=')) {
            return Token(TokenType::Assign, "=");
        }
        if (this->Switch1('<')) {
            return Token(TokenType::Less, "<");
        }
        if (this->Switch1('>')) {
            return Token(TokenType::Greater, ">");
        }
        if (this->Switch1('!')) {
            return Token(TokenType::Not, "!");
        }
        if (this->Switch1('&')) {
            return Token(TokenType::And, "&");
        }
        if (this->Switch1('|')) {
            return Token(TokenType::Or, "|");
        }
        if (this->Switch1('^')) {
            return Token(TokenType::Xor, "^");
        }
        if (this->Switch1('~')) {
            return Token(TokenType::Xor, "~");
        }
        if (this->Switch1('?')) {
            return Token(TokenType::Question, "?");
        }
        if (this->Switch1(':')) {
            return Token(TokenType::Colon, ":");
        }
        if (this->Switch1(';')) {
            return Token(TokenType::Semicolon, ";");
        }
        if (this->Switch1(',')) {
            return Token(TokenType::Comma, ",");
        }
        if (this->Switch1('.')) {
            return Token(TokenType::Dot, ".");
        }
        if (this->Switch1('(')) {
            return Token(TokenType::LParen, "(");
        }
        if (this->Switch1(')')) {
            return Token(TokenType::RParen, ")");
        }
        if (this->Switch1('[')) {
            return Token(TokenType::LBracket, "[");
        }
        if (this->Switch1(']')) {
            return Token(TokenType::RBracket, "]");
        }
        if (this->Switch1('{')) {
            return Token(TokenType::LBrace, "{");
        }
        if (this->Switch1('}')) {
            return Token(TokenType::RBrace, "}");
        }
        this->Advance();
    }
    return Token(TokenType::EndOfFile, "EOF", this->column, this->line);
}
Token Lexer::ParseIdentifier() {
    std::string identifier;
    while (IsAlphaNumeric()) {
        identifier += this->currentChar;
        this->Advance();
    }
    if (identifier == "if") {
        return Token(TokenType::If, "if");
    }
    if (identifier == "elif") {
        return Token(TokenType::Elif, "elif");
    }
    if (identifier == "else") {
        return Token(TokenType::Else, "else");
    }
    if (identifier == "while") {
        return Token(TokenType::While, "while");
    }
    if (identifier == "for") {
        return Token(TokenType::For, "for");
    }
    if (identifier == "break") {
        return Token(TokenType::Break, "break");
    }
    if (identifier == "continue") {
        return Token(TokenType::Continue, "continue");
    }
    if (identifier == "ret") {
        return Token(TokenType::Ret, "ret");
    }
    if (identifier == "class") {
        return Token(TokenType::Class, "class");
    }
    if (identifier == "type") {
        return Token(TokenType::Type, "type");
    }
    if (identifier == "const") {
        return Token(TokenType::Const, "const");
    }
    if (identifier == "let") {
        return Token(TokenType::Let, "let");
    }
    if (identifier == "mut") {
        return Token(TokenType::Mut, "mut");
    }
    if (identifier == "import") {
        return Token(TokenType::Import, "import");
    }
    if (identifier == "pub") {
        return Token(TokenType::Pub, "pub");
    }
    if (identifier == "priv") {
        return Token(TokenType::Priv, "priv");
    }
    if (identifier == "static") {
        return Token(TokenType::Static, "static");
    }
    if (identifier == "virtual") {
        return Token(TokenType::Virtual, "virtual");
    }
    if (identifier == "new") {
        return Token(TokenType::New, "new");
    }
    if (identifier == "del") {
        return Token(TokenType::Del, "del");
    }
    if (identifier == "operator") {
        return Token(TokenType::Operator, "operator");
    }
    if (identifier == "enum") {
        return Token(TokenType::Enum, "enum");
    }
    return Token(TokenType::Ident, identifier, this->column, this->line);
}
Token Lexer::ParseNumber() {
    std::string number;
    while (this->IsDigit()) {
        number += this->currentChar;
        this->Advance();
    }
    if (this->currentChar == '.') {
        number += this->currentChar;
        this->Advance();
        while (this->IsDigit()) {
            number += this->currentChar;
            this->Advance();
        }
        return Token(TokenType::Float, number, this->column, this->line);
    }
    return Token(TokenType::Int, number, this->column, this->line);
}
Token Lexer::ParseString() {
    this->Advance();
    std::string string;
    bool isNotNull = true;
    bool isNotChar = true;
    while (isNotNull && isNotChar) {
        string += this->currentChar;
        this->Advance();
        isNotNull = this->IsNotNull();
        isNotChar = !this->IsChar('"');
    }
    this->Advance();
    return Token(TokenType::String, string, this->column, this->line);
}
Token Lexer::ParseChar() {
    this->Advance();
    std::string character;
    bool isNotNull = true;
    bool isNotChar = true;
    while (isNotNull && isNotChar) {
        character += this->currentChar;
        this->Advance();
        isNotNull = this->IsNotNull();
        isNotChar = !this->IsChar(39);
    }
    this->Advance();
    return Token(TokenType::Char, character, this->column, this->line);
}
void Lexer::SkipComment() {
    bool isNotNewline = true;
    bool isNotNull = true;
    while (isNotNull && isNotNewline) {
        this->Advance();
        isNotNull = this->IsNotNull();
        isNotNewline = !this->IsNewline();
    }
}
bool Lexer::Switch1(char c) {
    if (this->IsChar(c)) {
        this->Advance();
        return true;
    }
    return false;
}
bool Lexer::Switch2(char expected1, char expected2) {
    if (this->currentChar == expected1 && this->PeekEquals(expected2)) {
        this->Advance();
        this->Advance();
        return true;
    }
    return false;
}
bool Lexer::Switch3(char expected1, char expected2, char expected3) {
    bool const isExpected1 = this->currentChar == expected1;
    bool const isExpected2 = this->PeekEquals(expected2);
    bool const isExpected3 = this->PeekEquals(expected3);
    if (isExpected1 && isExpected2 && isExpected3) {
        this->Advance();
        this->Advance();
        this->Advance();
        return true;
    }
    return false;
}
bool Lexer::IsChar(char c) {
    return this->currentChar == c;
}
bool Lexer::IsNull() {
    return this->currentChar == '\0';
}
bool Lexer::IsNotNull() {
    return !this->IsNull();
}
bool Lexer::IsWhitespace() {
    return this->currentChar == ' ' || this->currentChar == '\t';
}
bool Lexer::IsAlpha() {
    return this->currentChar >= 'a' && this->currentChar <= 'z' || this->currentChar >= 'A' && this->currentChar <= 'Z';
}
bool Lexer::IsDigit() {
    return this->currentChar >= '0' && this->currentChar <= '9';
}
bool Lexer::IsAlphaNumeric() {
    bool const alpha = this->IsAlpha();
    bool const numeric = this->IsDigit();
    return alpha || numeric;
}
bool Lexer::IsNewline() {
    return this->currentChar == '\n';
}
