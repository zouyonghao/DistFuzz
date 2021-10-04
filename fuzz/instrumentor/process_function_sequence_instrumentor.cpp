#include <instrumentor.hpp>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

static const std::string blacklist_functions[] = {"asan", "gnu", "cxx", "llvm", "clang", "sanitizer"};
static const std::string whitelist_functions[] = {
    // "heartbeat",
    "snapshot", "request"};
static bool inject_function_exit = false;

class ProcessFunctionSequenceCoverageInstrumentor : public Instrumentor
{
private:
public:
    ProcessFunctionSequenceCoverageInstrumentor() : Instrumentor() {}
    ~ProcessFunctionSequenceCoverageInstrumentor();

    virtual std::vector<llvm::Instruction *> get_locations(llvm::Module *m) override
    {
        std::vector<llvm::Instruction *> locations;
        return locations;
    }

    virtual llvm::Function *get_function(llvm::Module *m) override { return nullptr; }

    llvm::Function *get_record_function(llvm::Module *m)
    {
        // Initialize paramater's type
        llvm::IntegerType *Integer = llvm::IntegerType::get(m->getContext(), 32);
        std::vector<llvm::Type *> FuncTy_args;
        FuncTy_args.push_back(Integer);
        // FuncTy_args.push_back(Integer);
        llvm::FunctionType *FuncTy =
            llvm::FunctionType::get(llvm::Type::getVoidTy(m->getContext()), FuncTy_args, false);

        llvm::Function *func = m->getFunction("FuncSequenceRecord");
        if (!func)
        {
            func = llvm::Function::Create(FuncTy, llvm::GlobalValue::ExternalLinkage, "FuncSequenceRecord", m);
            func->setCallingConv(llvm::CallingConv::C);
            llvm::AttributeList func_Add_PAL;
            func->setAttributes(func_Add_PAL);
        }

        return func;
    }

    void insert_record_function(llvm::Instruction *insert_point, unsigned int rand_num, llvm::Module *m)
    {
        auto insert_function = get_record_function(m);
        llvm::ConstantInt *rand_int = llvm::ConstantInt::get(m->getContext(), llvm::APInt(32, rand_num));
        // Prepare parameters for MyCovCal
        std::vector<llvm::Value *> parameters;
        parameters.push_back(rand_int);

        llvm::CallInst *mycall = llvm::CallInst::Create(insert_function, parameters, "", insert_point);
        mycall->setCallingConv(llvm::CallingConv::C);
        mycall->setTailCall(false);
    }

    virtual void do_instrument(llvm::Module *m) override
    {
        for (auto &func_i : *m)
        {
            for (auto &bbI : func_i)
            {
                for (auto &inst : bbI)
                {
                    if (llvm::CallInst *ci = llvm::dyn_cast<llvm::CallInst>(&inst))
                    {
                        if (llvm::Function *called = ci->getCalledFunction())
                        {
                            bool is_in_blacklist = false;
                            bool is_in_whitelist = false;
                            for (auto func_name : whitelist_functions)
                            {
                                if (called->getName().find(func_name) != std::string::npos)
                                {
                                    is_in_whitelist = true;
                                    llvm::errs() << "we have a called function in white list, it's "
                                                 << called->getName() << " called in " << func_i.getName() << "\n";
                                    break;
                                }
                            }
                            if (!is_in_whitelist)
                            {
                                continue;
                            }
                            for (auto func_name : blacklist_functions)
                            {
                                if (called->getName().find(func_name) != std::string::npos)
                                {
                                    is_in_blacklist = true;
                                    break;
                                }
                            }
                            if (is_in_blacklist)
                            {
                                continue;
                            }
                            unsigned int rand_num = rand();
                            insert_record_function(&inst, rand_num, m);
                        }
                    }
                }
            }
        }
    }
};

REGISTER_INSTRUMENTOR(ProcessFunctionSequenceCoverageInstrumentor, new ProcessFunctionSequenceCoverageInstrumentor());