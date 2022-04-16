class ExprAST {
public:
    virtual ~ExprAST() {}
};

// Expression class for numeric literals
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double V) : Val(V) {}
};

class VariableExprAST : public ExprAST {
    string Name;

public:
    VariableExprAST(const string &Name) : Name(Name) {}
};

class BinaryExprAST : public ExprAST {
    char Op;
    unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char op, unique_ptr<ExprAST> LHS, unique_ptr<ExprAST> RHS)
        : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExprAST {
    string Callee;
    vector<unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee, vector<unique_ptr<ExprAST>> Args)
        : Callee(Callee), Args(std::move(Args)) {}
};

class PrototypeAST {
    string Name;
    vector<string> Args;

public:
    PrototypeAST(const string &name, vector<string> Args)
        : Name(name), Args(std::move(Args)) {}

    const string &getName() const { return Name; }
};

class FunctionAST {
    unique_ptr<PrototypeAST> Proto;
    unique_ptr<ExprAST> Body;

public:
    FunctionAST(unique_ptr<PrototypeAST> Proto, unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

