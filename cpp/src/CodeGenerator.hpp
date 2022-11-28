#pragma once
#include "iostream"
#include "tuple"
#include "AST.hpp"
class IRFunction {
public: std::string name;
public: IRFunction(std::string name);
public: [[nodiscard]] std::string ToString() const ;
};
class CodeGenerator {
public: [[nodiscard]] std::tuple<std::string, std::string> Generate(ASTNode* ast);
};
