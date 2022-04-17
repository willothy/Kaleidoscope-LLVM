#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "header/token.h"
#include "header/parser.h"

using namespace Tokenizer;
using namespace Parser;

// Top-level parsing
static void HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else {
        GetNextToken(); // Skip token for error recovery
    }
}

static void HandleExtern() {
    if (ParseExtern()) {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else {
        GetNextToken(); // Skip token for error recovery
    }
}

static void HandleTopLevelExpression() {
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else {
        GetNextToken(); // Skip token for error recovery
    }
}

static void MainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (CurTok) {
        case tok_eof:
            return;
        case ';':
            GetNextToken();
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

int main() {

    fprintf(stderr, "ready> ");
    Parser::GetNextToken();

    MainLoop();

    return 0;
}