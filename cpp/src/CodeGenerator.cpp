#include "CodeGenerator.hpp"
#include "iostream"
#include "tuple"
#include "AST.hpp"
IRFunction::IRFunction(std::string name) {
    this->name = name;
}
std::string IRFunction::ToString()  const {
    return "IRFunction(" + this->name + ")";
}
std::tuple<std::string, std::string> CodeGenerator::Generate(ASTNode* ast) {
    std::cout << "Generating code..." << std::endl;
    IRFunction irFunction = IRFunction("main");
    std::string code = irFunction.ToString();
    std::string header = "";
    return std::make_tuple(code, header);
}
