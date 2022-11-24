#pragma once
#include "iostream"
#include "vector"
#include "string"
#include "Token.hpp"
class Lexer {
protected: std::string source;
protected: int index;
protected: int column;
protected: int line;
protected: std::vector<Token> tokens;
protected: char currentChar;
public: Lexer(const std::string& source);
protected: [[nodiscard]] char Peek();
protected: [[nodiscard]] char Peek2();
protected: [[nodiscard]] bool PeekEquals(char expected);
protected: [[nodiscard]] bool Peek2Equals(char expected);
protected: auto Advance();
protected: auto SkipWhitespace();
public: [[nodiscard]] std::vector<Token> Parse();
protected: [[nodiscard]] Token ParseToken();
protected: [[nodiscard]] Token ParseIdentifier();
protected: [[nodiscard]] Token ParseNumber();
protected: [[nodiscard]] Token ParseString();
protected: [[nodiscard]] Token ParseChar();
protected: void SkipComment();
protected: [[nodiscard]] bool Switch1(char c);
protected: [[nodiscard]] bool Switch2(char expected1, char expected2);
protected: [[nodiscard]] bool Switch3(char expected1, char expected2, char expected3);
protected: [[nodiscard]] bool IsChar(char c);
protected: [[nodiscard]] bool IsNull();
protected: [[nodiscard]] bool IsNotNull();
protected: [[nodiscard]] bool IsWhitespace();
protected: [[nodiscard]] bool IsAlpha();
protected: [[nodiscard]] bool IsDigit();
protected: [[nodiscard]] bool IsAlphaNumeric();
protected: [[nodiscard]] bool IsNewline();
};
