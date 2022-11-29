#include "Bic.hpp"
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
int main(int argc, char* argv[]) {
    std::ifstream file = std::ifstream("test.bic");
    std::string fileContents = "";
    std::string line = "";
    while (std::getline(file, line)) {
        fileContents += line + "\n";
    }
    Lexer* lexer = new Lexer(fileContents);
    Parser* parser = new Parser(lexer);
    ASTNode* const root = parser->Parse();
    IRGenerator* irGenerator = new IRGenerator();
    IRNode* const ir = irGenerator->Generate(root);
    CodeGenerator* codeGenerator = new CodeGenerator();
    codeGenerator->Generate(ir);
    std::cout << "Source:" << std::endl;
    std::cout << codeGenerator->sourceBuffer << std::endl;
    std::cout << "Header:" << std::endl;
    std::cout << codeGenerator->headerBuffer << std::endl;
    delete lexer;
    delete parser;
    delete root;
    delete ir;
    delete irGenerator;
    delete codeGenerator;
    return 0;
}
