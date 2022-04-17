#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "ast.h"
#include "token.h"

namespace Parser {
    using namespace AST;
    static int CurTok;
    int GetNextToken() {
        return CurTok = Tokenizer::gettok();
    }

    int GetTokPrecedence() {
        if (!isascii(CurTok)) return -1;

        switch (CurTok) {
        case '<':
        case '>':
            return 10;
        case '+':
        case '-':
            return 20;
        case '*':
            //case '/':
            return 40;
        default:
            return -1;
        }
    }

    static std::unique_ptr<ExprAST> ParseExpression();

    std::unique_ptr<ExprAST> LogError(const char* Str) {
        fprintf(stderr, "LogError: %s\n", Str);
        return nullptr;
    }

    std::unique_ptr<PrototypeAST> LogErrorP(const char* Str) {
        LogError(Str);
        return nullptr;
    }

    static std::unique_ptr<ExprAST> ParseNumberExpr() {
        // Not sure why auto is used here. I'll leave it for now, but maybe unique_ptr<ExprAST> instead?
        auto Result = std::make_unique<NumberExprAST>(Tokenizer::NumVal);
        GetNextToken();
        return std::move(Result);
    }

    static std::unique_ptr<ExprAST> ParseParenExpr() {
        GetNextToken(); // eat (
        auto V = ParseExpression();
        if (!V)
            return nullptr;

        if (CurTok != ')')
            return LogError("Expected ')'");
        GetNextToken(); // eat )
        return V;
    }

    static std::unique_ptr<ExprAST> ParseIdentifierOrCallExpr() {
        std::string IdName = Tokenizer::IdentifierStr;

        GetNextToken(); // Eat identifier

        if (CurTok != '(')
            return std::make_unique<VariableExprAST>(IdName);

        // Call    
        GetNextToken(); // Eat (
        std::vector<std::unique_ptr<ExprAST>> Args;
        if (CurTok != ')') {
            while (true) {
                if (auto Arg = ParseExpression())
                    Args.push_back(std::move(Arg));
                else
                    return nullptr;

                if (CurTok == ')')
                    break;
                if (CurTok != ',')
                    return LogError("Expected ')' or ',' in argument list");
                GetNextToken(); // Eat ,
            }
        }

        // eat )
        GetNextToken();
        return std::make_unique<CallExprAST>(IdName, std::move(Args));
    }

    static std::unique_ptr<ExprAST> ParsePrimary() {
        switch (CurTok) {
        default:
            return LogError("Unknown token when expecting an expression");
        case Tokenizer::tok_identifier:
            return ParseIdentifierOrCallExpr();
        case Tokenizer::tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
        }
    }

    static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
        // If this is a binop, find its precedence
        while (true) {
            int TokPrec = GetTokPrecedence();

            if (TokPrec < ExprPrec)
                return LHS;

            int BinOp = CurTok;
            GetNextToken(); // eat binop

            auto RHS = ParsePrimary();
            if (!RHS) return nullptr;

            int NextPrec = GetTokPrecedence();
            if (TokPrec < NextPrec) {
                RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
                if (!RHS) return nullptr;
            }
            LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
        }
    }

    static std::unique_ptr<ExprAST> ParseExpression() {
        auto LHS = ParsePrimary();

        if (!LHS)
            return nullptr;

        return ParseBinOpRHS(0, std::move(LHS));;
    }

    static std::unique_ptr<PrototypeAST> ParsePrototype() {
        if (CurTok != Tokenizer::tok_identifier)
            return LogErrorP("Expected function name in prototype");

        std::string FnName = Tokenizer::IdentifierStr;
        GetNextToken();

        if (CurTok != '(') {
            std::cout << CurTok << std::endl;
            return LogErrorP("Expected '(' in function prototype.");
        }

        // read list of argument names
        std::vector<std::string> ArgNames;
        while (GetNextToken() == Tokenizer::tok_identifier)
            ArgNames.push_back(Tokenizer::IdentifierStr);
        if (CurTok != ')')
            return LogErrorP("Expected ')' in function prototype.");

        GetNextToken(); // eat )

        return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
    }

    static std::unique_ptr<FunctionAST> ParseDefinition() {
        GetNextToken(); // eat def
        auto Proto = ParsePrototype();
        if (!Proto) return nullptr;

        if (auto E = ParseExpression())
            return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        return nullptr;
    }

    static std::unique_ptr<FunctionAST> ParseTopLevelExpr() {
        if (auto E = ParseExpression()) {
            // Make anonymous proto
            auto Proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
            return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        }
        return nullptr;
    }

    static std::unique_ptr<PrototypeAST> ParseExtern() {
        GetNextToken();
        return ParsePrototype();
    }
}