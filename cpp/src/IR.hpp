#pragma once
#include "iostream"
#include "vector"
#include "tuple"
#include "AST.hpp"
class IRNode {
public: IRNode();
public: [[nodiscard]] virtual std::string ToString() const ;
};
class IRProgram : public IRNode {
public: std::vector<IRNode*> decls;
public: IRProgram(std::vector<IRNode*> declList);
public: [[nodiscard]] std::string ToString() const ;
};
class IRBody : public IRNode {
public: std::vector<IRNode*> decls;
public: auto IRProgram();
public: [[nodiscard]] std::string ToString() const ;
};
class IRExpr : public IRNode {
public: std::string expr;
public: IRExpr(std::string expr);
public: [[nodiscard]] std::string ToString() const ;
};
class IRFunction : public IRNode {
public: std::string name;
public: std::string returnType;
public: std::vector<std::tuple<std::string, std::string>> params;
public: IRBody* body;
public: int attr = Attribute::None;
public: [[nodiscard]] std::string ToString() const ;
};
class IRReturn : public IRNode {
public: IRExpr* expr;
public: [[nodiscard]] std::string ToString() const ;
};
class IRIf : public IRNode {
public: IRExpr* cond;
public: IRBody* body;
public: std::vector<IRIf*> elifs;
public: IRBody* elseBody;
public: [[nodiscard]] std::string ToString() const ;
};
class IRVariable : public IRNode {
public: std::string name;
public: std::string vType;
public: IRExpr* value;
public: int attr;
public: [[nodiscard]] std::string ToString() const ;
};
