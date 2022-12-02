#include "Token.hpp"
#include "iostream"
std::string TokenTypeToString(TokenType t) {
    if (t == TokenType::EndOfFile) {
        return "EndOfFile";
    }
    if (t == TokenType::Unknown) {
        return "Unknown";
    }
    if (t == TokenType::NewLine) {
        return "NewLine";
    }
    if (t == TokenType::Ident) {
        return "Ident";
    }
    if (t == TokenType::Int) {
        return "Int";
    }
    if (t == TokenType::Float) {
        return "Float";
    }
    if (t == TokenType::Char) {
        return "Char";
    }
    if (t == TokenType::String) {
        return "String";
    }
    if (t == TokenType::True) {
        return "True";
    }
    if (t == TokenType::False) {
        return "False";
    }
    if (t == TokenType::Null) {
        return "Null";
    }
    if (t == TokenType::StartKeyword) {
        return "StartKeyword";
    }
    if (t == TokenType::If) {
        return "If";
    }
    if (t == TokenType::Elif) {
        return "Elif";
    }
    if (t == TokenType::Else) {
        return "Else";
    }
    if (t == TokenType::While) {
        return "While";
    }
    if (t == TokenType::For) {
        return "For";
    }
    if (t == TokenType::In) {
        return "In";
    }
    if (t == TokenType::Break) {
        return "Break";
    }
    if (t == TokenType::Continue) {
        return "Continue";
    }
    if (t == TokenType::Ret) {
        return "Ret";
    }
    if (t == TokenType::Type) {
        return "Type";
    }
    if (t == TokenType::Const) {
        return "Const";
    }
    if (t == TokenType::Let) {
        return "Let";
    }
    if (t == TokenType::Mut) {
        return "Mut";
    }
    if (t == TokenType::Import) {
        return "Import";
    }
    if (t == TokenType::Class) {
        return "Class";
    }
    if (t == TokenType::Pub) {
        return "Pub";
    }
    if (t == TokenType::Priv) {
        return "Priv";
    }
    if (t == TokenType::Static) {
        return "Static";
    }
    if (t == TokenType::Virtual) {
        return "Virtual";
    }
    if (t == TokenType::New) {
        return "New";
    }
    if (t == TokenType::Del) {
        return "Del";
    }
    if (t == TokenType::Operator) {
        return "Operator";
    }
    if (t == TokenType::Enum) {
        return "Enum";
    }
    if (t == TokenType::EndKeyword) {
        return "EndKeyword";
    }
    if (t == TokenType::Add) {
        return "Add";
    }
    if (t == TokenType::Sub) {
        return "Sub";
    }
    if (t == TokenType::Mul) {
        return "Mul";
    }
    if (t == TokenType::Div) {
        return "Div";
    }
    if (t == TokenType::Mod) {
        return "Mod";
    }
    if (t == TokenType::Not) {
        return "Not";
    }
    if (t == TokenType::BitNot) {
        return "BitNot";
    }
    if (t == TokenType::Amp) {
        return "Amp";
    }
    if (t == TokenType::Pipe) {
        return "Pipe";
    }
    if (t == TokenType::Xor) {
        return "Xor";
    }
    if (t == TokenType::Less) {
        return "Less";
    }
    if (t == TokenType::Greater) {
        return "Greater";
    }
    if (t == TokenType::And) {
        return "And";
    }
    if (t == TokenType::Or) {
        return "Or";
    }
    if (t == TokenType::LShift) {
        return "LShift";
    }
    if (t == TokenType::RShift) {
        return "RShift";
    }
    if (t == TokenType::Assign) {
        return "Assign";
    }
    if (t == TokenType::AddAssign) {
        return "AddAssign";
    }
    if (t == TokenType::SubAssign) {
        return "SubAssign";
    }
    if (t == TokenType::MulAssign) {
        return "MulAssign";
    }
    if (t == TokenType::DivAssign) {
        return "DivAssign";
    }
    if (t == TokenType::ModAssign) {
        return "ModAssign";
    }
    if (t == TokenType::AndAssign) {
        return "AndAssign";
    }
    if (t == TokenType::OrAssign) {
        return "OrAssign";
    }
    if (t == TokenType::XorAssign) {
        return "XorAssign";
    }
    if (t == TokenType::LShiftAssign) {
        return "LShiftAssign";
    }
    if (t == TokenType::RShiftAssign) {
        return "RShiftAssign";
    }
    if (t == TokenType::Equal) {
        return "Equal";
    }
    if (t == TokenType::NotEqual) {
        return "NotEqual";
    }
    if (t == TokenType::LessEqual) {
        return "LessEqual";
    }
    if (t == TokenType::GreaterEqual) {
        return "GreaterEqual";
    }
    if (t == TokenType::Inc) {
        return "Inc";
    }
    if (t == TokenType::Dec) {
        return "Dec";
    }
    if (t == TokenType::Dot) {
        return "Dot";
    }
    if (t == TokenType::Comma) {
        return "Comma";
    }
    if (t == TokenType::Colon) {
        return "Colon";
    }
    if (t == TokenType::Semicolon) {
        return "Semicolon";
    }
    if (t == TokenType::Question) {
        return "Question";
    }
    if (t == TokenType::At) {
        return "At";
    }
    if (t == TokenType::Hash) {
        return "Hash";
    }
    if (t == TokenType::Dollar) {
        return "Dollar";
    }
    if (t == TokenType::LParen) {
        return "LParen";
    }
    if (t == TokenType::RParen) {
        return "RParen";
    }
    if (t == TokenType::LBracket) {
        return "LBracket";
    }
    if (t == TokenType::RBracket) {
        return "RBracket";
    }
    if (t == TokenType::LBrace) {
        return "LBrace";
    }
    if (t == TokenType::RBrace) {
        return "RBrace";
    }
    if (t == TokenType::Arrow) {
        return "Arrow";
    }
    if (t == TokenType::Ellipsis) {
        return "Ellipsis";
    }
    if (t == TokenType::ScopeResolution) {
        return "ScopeResolution";
    }
    return "Mmmm...";
}
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
