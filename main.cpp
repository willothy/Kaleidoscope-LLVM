#include <iostream>

#include "parser.cpp"

using std::cout;

// Top-level parsing
static void HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        getNextToken(); // Skip token for error recovery
    }
}

static void HandleExtern() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        getNextToken(); // Skip token for error recovery
    }
}

static void HandleTopLevelExpression() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        getNextToken(); // Skip token for error recovery
    }
}

static void MainLoop() {
    while(true) {
        fprintf(stderr, "ready> ");
        switch(CurTok) {
            case tok_eof: return;
            case ';':
                getNextToken();
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
    getNextToken();

    MainLoop();
    
    return 0;
}