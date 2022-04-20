#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "../header/parser.hpp"


class Driver {
public:
    Parser parser{};

    // Top-level parsing
    void HandleDefinition() {
        if (this->parser.ParseDefinition()) {
            fprintf(stderr, "Parsed a function definition.\n");
        }
        else {
            this->parser.GetNextToken(); // Skip token for error recovery
        }
    }

    void HandleExtern() {
        if (this->parser.ParseExtern()) {
            fprintf(stderr, "Parsed an extern definition.\n");
        }
        else {
            this->parser.GetNextToken(); // Skip token for error recovery
        }
    }

    void HandleTopLevelExpression() {
        if (this->parser.ParseTopLevelExpr()) {
            fprintf(stderr, "Parsed a top level expr.\n");
        }
        else {
            this->parser.GetNextToken(); // Skip token for error recovery
        }
    }
public:
    void Init() {
        fprintf(stderr, "ready> ");
        this->parser.GetNextToken();
    }

    void MainLoop() {
        while (true) {
            fprintf(stderr, "ready> ");
            switch (this->parser.CurTok) {
            case tok_eof:
                return;
            case ';':
                this->parser.GetNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
            }
        }
    }
};

int main(int argc, char** argv) {
    Driver driver{};
    
    driver.Init();

    driver.MainLoop();

    return 0;
}