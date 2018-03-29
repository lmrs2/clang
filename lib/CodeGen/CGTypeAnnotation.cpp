// Experimental type annotation support for Clang.
// By Adrian Sampson <asampson@cs.washington.edu>.

#include "CodeGenModule.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Metadata.h"

using namespace clang;
using namespace clang::CodeGen;

void CodeGenModule::TADecorate(llvm::Instruction *Inst, clang::QualType Ty,
        uint8_t level) {
  if (!Inst)
    return;
//llvm::errs() << "TADecorate I\n";

  llvm::LLVMContext &Ctx = getLLVMContext();

  // Is this an annotated type?
  // TODO Try desugaring.
  if (auto *AT = llvm::dyn_cast<AnnotatedType>(Ty)) {
    StringRef Ann = AT->getAnnotation();

    // TODO add the "depth" of the annotation in the reference type chain.
    //llvm::Value *Args[2] = {
    //  llvm::MDString::get(Ctx, Ann),
    //  llvm::ConstantInt::get(llvm::Type::getInt8Ty(Ctx), level, false)
    //};
    llvm::Metadata *Args[2] = {
      llvm::MDString::get(Ctx, Ann),
      llvm::ConstantAsMetadata::get( llvm::ConstantInt::get(llvm::Type::getInt8Ty(Ctx), level, false) )
    };

    llvm::MDNode *node = llvm::MDNode::get(Ctx, Args);
    Inst->setMetadata("tyann", node);
  }
}

void CodeGenModule::TADecorate(llvm::Value *V, clang::QualType Ty,
        uint8_t level) {
	//llvm::errs() << "TADecorate V\n";
  if (!V)
    return;
  if (auto *Inst = dyn_cast<llvm::Instruction>(V)) {
    TADecorate(Inst, Ty, level);
  } else {
	  //llvm::errs() << "TADecorate ?\n";
	  //V->dump();
    // TODO maybe handle this case
  }
}

