#include "error.hpp"


std::unique_ptr<ExprAST> Error::LogError(const char* Str) {
    fprintf(stderr, "LogError: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> Error::LogErrorP(const char* Str) {
    LogError(Str);
    return nullptr;
}

llvm::Value* Error::LogErrorV(const char* Str) {
    LogError(Str);
    return nullptr;
}

llvm::Function* Error::LogErrorF(const char* Str) {
    LogError(Str);
    return nullptr;
}
