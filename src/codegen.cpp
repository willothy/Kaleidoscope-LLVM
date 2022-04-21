#include "parser.hpp"
#include "error.hpp"


llvm::Value* NumberExprAST::codegen() {
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
}

llvm::Value* VariableExprAST::codegen() {
    llvm::Value* V = NamedValues[Name];
    if (!V) Error::LogErrorV("Invalid binary operator");
    return V;
}

llvm::Value* BinaryExprAST::codegen() {
    llvm::Value* L = LHS->codegen();
    llvm::Value* R = RHS->codegen();
    if (!L || !R) return nullptr;

    switch(Op) {
    case '+':
        return Builder->CreateFAdd(L, R, "addtmp");
    case '-':
        return Builder->CreateFSub(L, R, "subtmp");
    case '*':
        return Builder->CreateFMul(L, R, "multmp");
    case '/':
        return Builder->CreateFDiv(L, R, "divtmp");
    case '<':
        L = Builder->CreateFCmpULT(L, R, "cmptmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    case '>':
        L = Builder->CreateFCmpUGT(L, R, "cmptmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    default:
        return Error::LogErrorV("Invalid binary operator");
    }
}

llvm::Value* CallExprAST::codegen() {
    llvm::Function* CalleeF = TheModule->getFunction(Callee);
    if (!CalleeF)
        return Error::LogErrorV("Unknown Function referenced");

    if (CalleeF->arg_size() != Args.size())
        return Error::LogErrorV("Incorrect number of arguments passed");

    std::vector<llvm::Value*> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->codegen());
        if (!ArgsV.back())
            return nullptr;
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

llvm::Function* PrototypeAST::codegen() {
    std::vector<llvm::Type*> Doubles(Args.size(), llvm::Type::getDoubleTy(*TheContext));

    llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);
    llvm::Function* F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());

    // Set names for arguments
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(Args[Idx++]);

    return F;
}

llvm::Function* FunctionAST::codegen() {
    llvm::Function* TheFunction = TheModule->getFunction(Proto->getName());

    if (!TheFunction) {
        TheFunction = Proto->codegen();
        if (!TheFunction)
            return nullptr;
    }

    if (!TheFunction->empty())
        return Error::LogErrorF("Function cannot be redefined");

    // Create basic block for insertion
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record function arguments in NamedValues map
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    if (llvm::Value* RetVal = Body->codegen()) {
        Builder->CreateRet(RetVal);

        // Validate the generated code (from llvm/IR/Verifier.h)
        llvm::verifyFunction(*TheFunction);
        
        return TheFunction;
    }

    // Error reading body, remove function
    TheFunction->eraseFromParent();
    return Error::LogErrorF("Error reading function body");
}


