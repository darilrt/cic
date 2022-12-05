#include "Core.hpp"
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
IRNode* IRFromFile(std::string filename) {
    std::ifstream file = std::ifstream(filename);
    std::string fileContents = "";
    std::string line = "";
    std::vector<int> lines;
    while (std::getline(file, line)) {
        fileContents += line + "\n";
        lines.push_back(line.size());
    }
    file.close();
    Lexer* lexer = new Lexer(fileContents);
    Parser* parser = new Parser(lexer, lines);
    ASTNode* const root = parser->Parse();
    IRGenerator* irGenerator = new IRGenerator();
    IRNode* const ir = irGenerator->Generate(root);
    delete lexer;
    delete parser;
    delete root;
    delete irGenerator;
    return ir;
}
void SaveIRToFile(IRNode* ir, const std::string& outputDir, const std::string& filename) {
    std::string newFilename = filename;
    int i = 0;
    while ((i) < (newFilename.size())) {
        if (newFilename[i] == 92) {
            newFilename[i] = '/';
        }
        i++;
    }
    std::string stem = newFilename.substr((newFilename.find_last_of("/")) + 1, newFilename.size());
    stem = stem.substr(0, stem.find_last_of("."));
    CodeGenerator* codeGenerator = new CodeGenerator();
    codeGenerator->Generate(ir);
    std::string output = outputDir;
    if (outputDir == "stdout") {
        std::cout << "// Header: " << filename << std::endl;
        std::cout << "#pragma once" << std::endl;
        std::cout << codeGenerator->headerBuffer;
        std::cout << std::endl << "// Source: " << filename << std::endl;
        std::cout << "#include \"" << stem << ".hpp\"" << std::endl;
        std::cout << codeGenerator->sourceBuffer;
    } else {
        if (output == "") {
            output = stem;
        } else {
            output = outputDir + "/" + stem;
            i = 0;
            while ((i) < (output.size())) {
                if (output[i] == 92) {
                    output[i] = '/';
                }
                i++;
            }
        }
        std::ofstream outputSource = std::ofstream(output + ".cpp");
        outputSource << "#include \"" << output << ".h\"" << std::endl;
        outputSource << codeGenerator->sourceBuffer;
        outputSource.close();
        std::ofstream outputHeader = std::ofstream(output + ".h");
        outputHeader << "#pragma once" << std::endl;
        outputHeader << codeGenerator->headerBuffer;
        outputHeader.close();
        std::cout << "Wrote " << output << ".h and " << output << ".cpp" << std::endl;
    }
    delete codeGenerator;
}
