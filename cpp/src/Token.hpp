#pragma once
#include "iostream"
enum class TokenType {
    EndOfFile,
    Unknown,
    NewLine,
    Ident,
    Int,
    Float,
    Char,
    String,
    True,
    False,
    Null,
    StartKeyword,
    If,
    Elif,
    Else,
    While,
    For,
    In,
    Break,
    Continue,
    Ret,
    Type,
    Const,
    Let,
    Mut,
    Import,
    Class,
    Pub,
    Priv,
    Static,
    Virtual,
    New,
    Del,
    Operator,
    Enum,
    EndKeyword,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Not,
    BitNot,
    Amp,
    Pipe,
    Xor,
    Less,
    Greater,
    And,
    Or,
    LShift,
    RShift,
    Assign,
    AddAssign,
    SubAssign,
    MulAssign,
    DivAssign,
    ModAssign,
    AndAssign,
    OrAssign,
    XorAssign,
    LShiftAssign,
    RShiftAssign,
    Equal,
    NotEqual,
    LessEqual,
    GreaterEqual,
    Inc,
    Dec,
    Dot,
    Comma,
    Colon,
    Semicolon,
    Question,
    At,
    Hash,
    Dollar,
    LParen,
    RParen,
    LBracket,
    RBracket,
    LBrace,
    RBrace,
    Arrow,
    Ellipsis,
    ScopeResolution,
};
[[nodiscard]] std::string TokenTypeToString(TokenType t);
class Token {
public: TokenType tokenType;
public: std::string literal;
public: int column;
public: int line;
public: Token();
public: Token(TokenType t, const std::string& literal);
public: Token(TokenType t, const std::string& literal, int column, int line);
public: [[nodiscard]] std::string ToString() const ;
};
