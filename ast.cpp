class ExprAST {
public:
    virtual ~ExprAST() {}
};

// Expression class for numeric literals
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
};