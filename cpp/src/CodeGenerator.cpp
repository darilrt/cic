#include "CodeGenerator.hpp"
#include "iostream"
#include "IR.hpp"
#include "Common.hpp"
void CodeGenerator::Generate(IRNode* ir) {
    std::cout << "Generating code..." << std::endl;
    if (IsA<IRProgram>(ir)) {
        GenerateProgram(As<IRProgram>(ir));
    }
}
void CodeGenerator::Indent() {
    int i = 0;
    while ((i) < indentCount) {
        indent += "    ";
        i++;
    }
}
void CodeGenerator::PushIndent() {
    indentCount++;
    Indent();
}
void CodeGenerator::PopIndent() {
    indentCount--;
    Indent();
}
void CodeGenerator::GenerateProgram(IRProgram* node) {
}
