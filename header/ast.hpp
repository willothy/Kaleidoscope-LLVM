#pragma once

#include <string>
#include <vector>
#include <memory>


class ExprAST {
public:
    virtual ~ExprAST();
};

// Expression class for numeric literals
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double V);
};

class VariableExprAST : public ExprAST {
    std::string Name;

public:
    VariableExprAST(const std::string& Name);
};

class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS);
};

class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector< std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string& Callee, std::vector< std::unique_ptr<ExprAST>> Args);
};

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string& name, std::vector<std::string> Args);

    const std::string& getName() const;
};

class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body);
};

