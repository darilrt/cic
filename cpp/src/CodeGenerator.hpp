#pragma once
#include "iostream"
#include "IR.hpp"
#include "Common.hpp"
template <typename T> [[nodiscard]] bool IsA(IRNode* node){
    return (dynamic_cast<T*>(node)) != 0;
}
template <typename T> [[nodiscard]] T* As(IRNode* node){
    return dynamic_cast<T*>(node);
}
class CodeGenerator {
public: std::string sourceBuffer = "";
public: std::string headerBuffer = "";
public: int indentCount = 0;
public: std::string indent = "";
public: void Generate(IRNode* ir);
protected: void Indent();
protected: void PushIndent();
protected: void PopIndent();
protected: void GenerateProgram(IRProgram* node);
protected: void GenerateDeclaration(IRNode* node);
protected: void GenerateFunction(IRFunction* node);
};
