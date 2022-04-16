#include <memory>
#include <vector>
#include <string>
#include <map>

#include "ast.cpp"
#include "token.cpp"

using std::unique_ptr;
using std::vector;
using std::string;

static int CurTok;
static int getNextToken() {
    return CurTok = gettok();
}

unique_ptr<ExprAST> LogError(const char* Str) {
    fprintf(stderr, "LogError: %s\n", Str);
    return nullptr;
}

unique_ptr<PrototypeAST> LogErrorP(const char* Str) {
    LogError(Str);
    return nullptr;
}

static int GetTokPrecedence() {
    if (!isascii(CurTok)) return -1;

    switch(CurTok) {
        default: 
            return -1;
        case '<':
        case '>': 
            return 10;
        case '+': 
        case '-': 
            return 20;
        case '*': 
        case '/':
            return 40;
    }
}

static unique_ptr<ExprAST> ParseNumberExpr() {
    // Not sure why auto is used here. I'll leave it for now, but maybe unique_ptr<ExprAST> instead?
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

static unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // eat (
    auto V = ParseExpression();
    if (!V) 
        return nullptr;

    if (CurTok != ')') 
        return LogError("Expected ')', got " + CurTok);
    getNextToken(); // eat )
    return V;
}

static unique_ptr<ExprAST> ParseIdentifierOrCallExpr() {
    string IdName = IdentifierStr;

    getNextToken(); // Eat identifier

    // Call
    if (CurTok == '(') {
        getNextToken(); // Eat (
        vector<unique_ptr<ExprAST>> Args;
        while(1) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' got " + CurTok);
            getNextToken(); // Eat ,
        }

        // eat )
        getNextToken();
        return std::make_unique<CallExprAST>(IdName, std::move(Args));
    }
    // Simple Identifier reference
    return std::make_unique<VariableExprAST>(IdName);
}

static unique_ptr<ExprAST> ParsePrimary() {
    switch(CurTok) {
        default:
            return LogError("Unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierOrCallExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}

static unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, unique_ptr<ExprAST> LHS) {
    // If this is a binop, find its precedence
    while(1) {
        int TokPrec = GetTokPrecedence();

        if (TokPrec < ExprPrec)
            return LHS;

        int BinOp = CurTok;
        getNextToken(); // eat binop

        auto RHS = ParsePrimary();
        if (RHS) {
            int NextPrec = GetTokPrecedence();
            if (TokPrec < NextPrec) {
                RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
                if (!RHS) return nullptr;
            }
            LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
        }
        return nullptr;
    }
}

static unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    
    if (!LHS)
        return ParseBinOpRHS(0, std::move(LHS));

    return nullptr;
}

static unique_ptr<PrototypeAST> ParsePrototype() {
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");
    
    string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in function prototype.");

    // read list of argument names
    vector<string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in function prototype.");

    getNextToken(); // eat )

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

static unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken(); // eat def
    auto Proto = ParsePrototype();
    if (!Proto) return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

static std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken();
    return ParsePrototype();
}

static unique_ptr<FunctionAST> ParseTopLevelExpr() {
    auto E = ParseExpression();
    if (E) {
        // Make anonymous proto
        auto Proto = std::make_unique<PrototypeAST>("", vector<string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}
