#pragma once
#include "ast.hpp"

class Error {
public:
    static std::unique_ptr<ExprAST> LogError(const char* Str);

    static std::unique_ptr<PrototypeAST> LogErrorP(const char* Str);

    static llvm::Value* LogErrorV(const char* Str);

    static llvm::Function* LogErrorF(const char* Str);
};