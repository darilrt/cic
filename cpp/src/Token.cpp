#include "Token.hpp"
#include "iostream"
Token::Token() {
    this->tokenType = TokenType::Unknown;
    this->literal = "";
    this->column = 0;
    this->line = 0;
}
Token::Token(TokenType t, const std::string& literal) {
    this->tokenType = t;
    this->literal = literal;
}
Token::Token(TokenType t, const std::string& literal, int column, int line) {
    this->tokenType = t;
    this->literal = literal;
    this->column = column;
    this->line = line;
}
std::string Token::ToString()  const {
    return "'" + this->literal + "'";
}
