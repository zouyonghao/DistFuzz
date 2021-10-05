#include <instrumentor.hpp>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

static const std::string blacklist_functions[] = {"asan", "gnu", "cxx", "llvm", "clang", "sanitizer"};

class ProcessTickInstrumentor : public Instrumentor
{
private:
public:
    ProcessTickInstrumentor() : Instrumentor() {}
    ~ProcessTickInstrumentor();

    virtual std::vector<llvm::Instruction *> get_locations(llvm::Module *m) override
    {
        std::vector<llvm::Instruction *> locations;
        for (auto &func_i : *m)
        {
            for (auto bbI = func_i.begin(), bbE = func_i.end(); bbI != bbE; ++bbI)
            {
                auto *basicBlocks = llvm::dyn_cast<llvm::BasicBlock>(bbI);
                llvm::Instruction *in_first = basicBlocks->getFirstNonPHI();
                if (in_first->getOpcode() == llvm::Instruction::LandingPad)
                {
                    in_first = in_first->getNextNode();
                }
                locations.push_back(in_first);
            }
        }
        return locations;
    }

    virtual llvm::Function *get_function(llvm::Module *m) override
    {
        // Initialize paramater's type
        llvm::IntegerType *Integer = llvm::IntegerType::get(m->getContext(), 32);
        std::vector<llvm::Type *> FuncTy_args;
        FuncTy_args.push_back(Integer);
        // FuncTy_args.push_back(Integer);
        llvm::FunctionType *FuncTy =
            llvm::FunctionType::get(llvm::Type::getVoidTy(m->getContext()), FuncTy_args, false);

        llvm::Function *func = m->getFunction("ProcessTick");
        if (!func)
        {
            func = llvm::Function::Create(FuncTy, llvm::GlobalValue::ExternalLinkage, "ProcessTick", m);
            func->setCallingConv(llvm::CallingConv::C);
        }
        llvm::AttributeList func_Add_PAL;
        func->setAttributes(func_Add_PAL);

        return func;
    }

    virtual void do_instrument(llvm::Module *m) override
    {

        for (auto &inst : this->get_locations(m))
        {
            auto insert_function = this->get_function(m);
            unsigned int rand_num = rand();
            llvm::ConstantInt *rand_int = llvm::ConstantInt::get(m->getContext(), llvm::APInt(32, rand_num));
            // Prepare parameters for MyCovCal
            std::vector<llvm::Value *> parameters;
            parameters.push_back(rand_int);

            llvm::CallInst *mycall = llvm::CallInst::Create(insert_function, parameters, "", inst);
            mycall->setCallingConv(llvm::CallingConv::C);
            mycall->setTailCall(false);
        }
    }
};

REGISTER_INSTRUMENTOR(ProcessTickInstrumentor, new ProcessTickInstrumentor());