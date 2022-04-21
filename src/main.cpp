#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>


#include "parser.hpp"

std::unique_ptr<llvm::LLVMContext> TheContext;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::Module> TheModule;
std::map<std::string, llvm::Value*> NamedValues;
//using namespace Kaleidoscope;

class Driver {
    // Top-level parsing
    void HandleDefinition() {
        if (auto FnAST = this->parser.ParseDefinition()) {
            if (auto* FnIR = FnAST->codegen()) {
                fprintf(stderr, "Parsed a function definition:\n");
                FnIR->print(llvm::errs());
                fprintf(stderr, "\n");
            }
        } else {
            this->parser.GetNextToken(); // Skip token for error recovery
        }
    }

    void HandleExtern() {
        if (auto ProtoAST = this->parser.ParseExtern()) {
            if (auto* FnIR = ProtoAST->codegen()) {
                fprintf(stderr, "Read extern:\n");
                FnIR->print(llvm::errs());
                fprintf(stderr, "\n");
            }
        } else {
            this->parser.GetNextToken(); // Skip token for error recovery
        }
    }

    void HandleTopLevelExpression() {
        if (auto FnAST = this->parser.ParseTopLevelExpr()) {
            if (auto *FnIR = FnAST->codegen()) {
                fprintf(stderr, "Read top-level expression:\n");
                FnIR->print(llvm::errs());
                fprintf(stderr, "\n");

                // Remove the anonymous expression.
                FnIR->eraseFromParent();
            }
        } else {
            this->parser.GetNextToken(); // Skip token for error recovery
        }
    }

    static void InitializeModule() {
        TheContext = std::make_unique<llvm::LLVMContext>();
        TheModule = std::make_unique<llvm::Module>("my jit module", *TheContext);

        // Create a new builder for the module
        Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
    }

    void MainLoop() {
        while (true) {
            switch (this->parser.CurTok) {
            case tok_eof:
                return;
            case ';':
                this->parser.GetNextToken();
                break;
            case tok_def:
                HandleDefinition();
                fprintf(stderr, "ready> ");
                break;
            case tok_extern:
                HandleExtern();
                fprintf(stderr, "ready> ");
                break;
            default:
                HandleTopLevelExpression();
                fprintf(stderr, "ready> ");
                break;
            }
        }
    }

public:
    Parser parser{};

    void Init() {
        fprintf(stderr, "ready> ");
        this->parser.GetNextToken();
        InitializeModule();
        MainLoop();
    }
};

int main(int argc, char** argv) {
    Driver driver{};
    
    driver.Init();

    return 0;
}