#pragma once
#include "iostream"
#include "tuple"
#include "AST.hpp"
class CodeGenerator {
public: [[nodiscard]] std::tuple<std::string, std::string> Generate(ASTNode* ast);
};
