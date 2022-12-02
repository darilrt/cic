#pragma once
#include "iostream"
#include "vector"
#include "fstream"
#include "tuple"
#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "AST.hpp"
#include "CodeGenerator.hpp"
#include "IRGenerator.hpp"
class ProgramOptions {
public: std::string input = "";
public: std::string output = "";
};
[[nodiscard]] ProgramOptions ParseArgs(std::vector<std::string> args);

