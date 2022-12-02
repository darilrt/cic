#include "CodeGenerator.hpp"
#include "iostream"
#include "IR.hpp"
#include "Common.hpp"
void CodeGenerator::Generate(IRNode* ir) {
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
    int i = 0;
    while ((i) < node->decls.size()) {
        GenerateDeclaration(node->decls[i]);
        i++;
    }
}
void CodeGenerator::GenerateDeclaration(IRNode* node) {
    if (IsA<IRFunction>(node)) {
        GenerateFunction(As<IRFunction>(node));
    }
    if (IsA<IRImport>(node)) {
        GenerateImport(As<IRImport>(node));
    }
    if (IsA<IRClass>(node)) {
        GenerateClass(As<IRClass>(node));
    }
}
void CodeGenerator::GenerateFunction(IRFunction* node) {
    std::string str = "";
    bool isTemplate = false;
    if ((node->templateParams.size()) > 0) {
        isTemplate = true;
        str += "template<";
        int i = 0;
        while ((i) < node->templateParams.size()) {
            std::string tType = std::get<0>(node->templateParams[i]);
            if ((tType) == "type") {
                tType = "typename";
            }
            str += tType + " " + std::get<1>(node->templateParams[i]);
            if ((i) < ((node->templateParams.size()) - 1)) {
                str += ", ";
            }
            i++;
        }
        str += "> ";
    }
    if ((node->attr & Attribute::Static) != 0) {
        str += "static ";
    }
    if ((node->attr & Attribute::Virtual) != 0) {
        str += "virtual ";
    }
    str += node->returnType + " " + node->name + "(";
    int i = 0;
    while ((i) < node->params.size()) {
        str += (std::get<0>(node->params[i])) + " " + (std::get<1>(node->params[i]));
        if ((i) < ((node->params.size()) - 1)) {
            str += ", ";
        }
        i++;
    }
    str += ")";
    if ((node->attr & Attribute::Const) != 0) {
        str += " const";
    }
    if (isTemplate) {
        str += " ";
        PushIndent();
        str += (node->body->ToString()) + "\n";
        PopIndent();
        headerBuffer += str;
    } else {
        headerBuffer += str + ";\n";
        str += " ";
        PushIndent();
        str += (node->body->ToString()) + "\n";
        PopIndent();
        sourceBuffer += str;
    }
}
void CodeGenerator::GenerateImport(IRImport* node) {
    headerBuffer += "#include \"" + node->path + '\"' + '\n';
}
void CodeGenerator::GenerateClass(IRClass* node) {
    std::string str;
    bool isTemplate = false;
    if ((node->templateParams.size()) > 0) {
        isTemplate = true;
        str += "template<";
        int i = 0;
        while ((i) < node->templateParams.size()) {
            std::string tType = std::get<0>(node->templateParams[i]);
            if ((tType) == "type") {
                tType = "typename";
            }
            str += tType + " " + std::get<1>(node->templateParams[i]);
            if ((i) < ((node->templateParams.size()) - 1)) {
                str += ", ";
            }
            i++;
        }
        str += "> ";
    }
    str += "class " + node->name;
    if ((node->inherits.size()) > 0) {
        str += " : ";
        int i = 0;
        while ((i) < node->inherits.size()) {
            str += node->inherits[i];
            if ((i) < ((node->inherits.size()) - 1)) {
                str += ", ";
            }
            i++;
        }
    }
    str += " {\n";
    PushIndent();
    int i = 0;
    while ((i) < node->inherits.size()) {
        i++;
    }
    PopIndent();
    str += "}";
    headerBuffer += str + ";\n";
}
