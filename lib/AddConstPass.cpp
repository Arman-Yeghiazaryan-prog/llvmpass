#include "AddConst.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"

using namespace llvm;

namespace addconst {
PreservedAnalyses AddConstPass::run(Function &F, FunctionAnalysisManager &FAM) {

    Module *M = F.getParent();

    FunctionType *funcType = FunctionType::get(Type::getVoidTy(M->getContext()), false);
    FunctionCallee helloFromCFunc = M->getOrInsertFunction("hello_from_c", funcType);

    for (BasicBlock &BB : F) {
        for (auto it = BB.begin(); it != BB.end(); ++it) {
            if (auto *Call = dyn_cast<CallInst>(&*it)) {

                Function *CalledFunc = Call->getCalledFunction();
                if (CalledFunc && CalledFunc->getName() == "printf") {
                    IRBuilder<> Builder(Call);
                    Builder.CreateCall(helloFromCFunc);
                    ++it; 
                    Builder.SetInsertPoint(&*it);
                    Builder.CreateCall(helloFromCFunc);
                }
            }
        }
    }
    auto PA = PreservedAnalyses::all();
    return PA;
}
} // namespace addconst
