#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "ast.hpp"

// Unknown tokens are represented by their ASCII code
enum Token {
	tok_eof = -1,
    tok_newline = -6,

	// commands
	tok_def = -2,
	tok_extern = -3,

	// primary
	tok_identifier = -4,
	tok_number = -5
};

class Parser {
public:
    std::string IdentifierStr;
	double NumVal;

    int CurTok;

	int gettok();

    int GetNextToken();

    int GetTokPrecedence();
    
    std::unique_ptr<ExprAST> ParseNumberExpr();

    std::unique_ptr<ExprAST> ParseParenExpr();

    std::unique_ptr<ExprAST> ParseIdentifierOrCallExpr();

    std::unique_ptr<ExprAST> ParsePrimary();

    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);

    std::unique_ptr<ExprAST> ParseExpression();

    std::unique_ptr<PrototypeAST> ParsePrototype();

    std::unique_ptr<FunctionAST> ParseDefinition();

    std::unique_ptr<FunctionAST> ParseTopLevelExpr();

    std::unique_ptr<PrototypeAST> ParseExtern();
};