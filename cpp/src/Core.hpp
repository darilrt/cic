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
[[nodiscard]] IRNode* IRFromFile(std::string filename);
void SaveIRToFile(IRNode* ir, const std::string& outputDir, const std::string& filename);
