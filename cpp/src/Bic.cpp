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
    CodeGenerator* codeGenerator = new CodeGenerator();
    std::tuple<std::string, std::string> const code = codeGenerator->Generate(root);
    std::cout << "Generated code:" << std::endl;
    std::cout << std::get<0>(code) << std::endl;
    std::cout << "Generated header:" << std::endl;
    std::cout << std::get<1>(code) << std::endl;
    delete lexer;
    delete parser;
    delete root;
    delete codeGenerator;
    return 0;
}
