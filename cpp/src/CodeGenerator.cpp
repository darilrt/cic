#include "CodeGenerator.hpp"
#include "iostream"
#include "tuple"
#include "AST.hpp"
std::tuple<std::string, std::string> CodeGenerator::Generate(ASTNode* ast) {
    std::cout << "Generating code..." << std::endl;
    std::string code = "";
    std::string header = "";
    return std::make_tuple(code, header);
}
