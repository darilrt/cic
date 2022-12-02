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
ProgramOptions ParseArgs(std::vector<std::string> args) {
    ProgramOptions options;
    int i = 1;
    while ((i) < (args.size())) {
        if (args[i] == "-o") {
            i++;
            options.output = args[i];
        } else {
            options.input = args[i];
        }
        i++;
    }
    return options;
}
int main(int argc, char* argv[]) {
    std::vector<std::string> args;
    int i = 0;
    while ((i) < argc) {
        args.push_back(argv[i]);
        i++;
    }
    ProgramOptions const options = ParseArgs(args);
    if (options.input == "") {
        std::cout << "No input file specified" << std::endl;
        return 1;
    }
    std::ifstream file = std::ifstream(options.input);
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
    std::cout << codeGenerator->headerBuffer << std::endl;
    std::cout << codeGenerator->sourceBuffer << std::endl;
    delete lexer;
    delete parser;
    delete root;
    delete ir;
    delete irGenerator;
    delete codeGenerator;
    return 0;
}
