#include "Bic.hpp"
#include "iostream"
#include "vector"
#include "fstream"
#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "AST.hpp"
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
    std::cout << root->ToString() << std::endl;
    return 0;
}
