#pragma once
#include "iostream"
#include "AST.hpp"
class CodeGenerator {
public: auto Generate(ASTNode* ast);
};
