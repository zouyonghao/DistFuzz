#include <instrumentor.hpp>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

static const std::string blacklist_functions[] = {"asan", "gnu", "cxx", "llvm", "clang", "sanitizer"};

class ProcessConcurrentCoverageInstrumentor : public Instrumentor
{
private:
public:
    ProcessConcurrentCoverageInstrumentor() : Instrumentor() {}
    ~ProcessConcurrentCoverageInstrumentor();

    virtual std::vector<llvm::Instruction *> get_locations(llvm::Module *m) override
    {
        std::vector<llvm::Instruction *> locations;
        return locations;
    }

    virtual llvm::Function *get_function(llvm::Module *m) override { return nullptr; }

    llvm::Function *get_func_enter_function(llvm::Module *m)
    {
        // Initialize paramater's type
        llvm::IntegerType *Integer = llvm::IntegerType::get(m->getContext(), 32);
        std::vector<llvm::Type *> FuncTy_args;
        FuncTy_args.push_back(Integer);
        // FuncTy_args.push_back(Integer);
        llvm::FunctionType *FuncTy =
            llvm::FunctionType::get(llvm::Type::getVoidTy(m->getContext()), FuncTy_args, false);

        llvm::Function *func = m->getFunction("FuncEnterRecord");
        if (!func)
        {
            func = llvm::Function::Create(FuncTy, llvm::GlobalValue::ExternalLinkage, "FuncEnterRecord", m);
            func->setCallingConv(llvm::CallingConv::C);
            llvm::AttributeList func_Add_PAL;
            func->setAttributes(func_Add_PAL);
        }

        return func;
    }

    llvm::Function *get_func_exit_function(llvm::Module *m)
    {
        // Initialize paramater's type
        llvm::IntegerType *Integer = llvm::IntegerType::get(m->getContext(), 32);
        std::vector<llvm::Type *> FuncTy_args;
        FuncTy_args.push_back(Integer);
        // FuncTy_args.push_back(Integer);
        llvm::FunctionType *FuncTy =
            llvm::FunctionType::get(llvm::Type::getVoidTy(m->getContext()), FuncTy_args, false);

        llvm::Function *func = m->getFunction("FuncExitRecord");
        if (!func)
        {
            func = llvm::Function::Create(FuncTy, llvm::GlobalValue::ExternalLinkage, "FuncExitRecord", m);
            func->setCallingConv(llvm::CallingConv::C);
            llvm::AttributeList func_Add_PAL;
            func->setAttributes(func_Add_PAL);
        }

        return func;
    }

    void insert_entry_function(std::vector<llvm::Instruction *> &insert_entry_function_points, unsigned int rand_num,
                               llvm::Module *m)
    {
        for (llvm::Instruction *i : insert_entry_function_points)
        {
            auto insert_function = get_func_enter_function(m);
            llvm::ConstantInt *rand_int = llvm::ConstantInt::get(m->getContext(), llvm::APInt(32, rand_num));
            // Prepare parameters for MyCovCal
            std::vector<llvm::Value *> parameters;
            parameters.push_back(rand_int);

            llvm::CallInst *mycall = llvm::CallInst::Create(insert_function, parameters, "", i);
            mycall->setCallingConv(llvm::CallingConv::C);
            mycall->setTailCall(false);
        }
    }

    void insert_exit_function(std::vector<llvm::Instruction *> &insert_exit_function_points, unsigned int rand_num,
                              llvm::Module *m)
    {
        for (llvm::Instruction *ri : insert_exit_function_points)
        {
            auto exit_function = get_func_exit_function(m);
            llvm::ConstantInt *rand_int = llvm::ConstantInt::get(m->getContext(), llvm::APInt(32, rand_num));
            // Prepare parameters for MyCovCal
            std::vector<llvm::Value *> parameters;
            parameters.push_back(rand_int);

            llvm::CallInst *mycall = llvm::CallInst::Create(exit_function, parameters, "", ri);
            mycall->setCallingConv(llvm::CallingConv::C);
            mycall->setTailCall(false);
        }
    }

    virtual void do_instrument(llvm::Module *m) override
    {
        for (auto &func_i : *m)
        {
            // llvm::errs() << "func_name: " << func_i.getName() << "\n";
            bool is_in_blacklist = false;
            bool has_entry = false;
            bool has_exit = false;
            std::vector<llvm::Instruction *> insert_entry_function_points;
            std::vector<llvm::Instruction *> insert_exit_function_points;
            if (func_i.isDeclaration())
            {
                continue;
            }
            for (auto func_name : blacklist_functions)
            {
                if (func_i.getName().find(func_name) != std::string::npos)
                {
                    is_in_blacklist = true;
                }
            }
            if (is_in_blacklist)
            {
                continue;
            }
            for (auto bbI = func_i.begin(), bbE = func_i.end(); bbI != bbE; ++bbI)
            {
                auto *basicBlock = llvm::dyn_cast<llvm::BasicBlock>(bbI);
                llvm::Instruction *in_first = basicBlock->getFirstNonPHI();
                if (in_first->getOpcode() == llvm::Instruction::LandingPad)
                {
                    in_first = in_first->getNextNode();
                }
                insert_entry_function_points.push_back(in_first);
                // llvm::errs() << "we have function entry!\n";
                has_entry = true;
                break; // break at first basic block
            }

            for (auto bbI = func_i.begin(), bbE = func_i.end(); bbI != bbE; ++bbI)
            {
                auto *basicBlock = llvm::dyn_cast<llvm::BasicBlock>(bbI);
                for (auto instI = basicBlock->begin(), instE = basicBlock->end(); instI != instE; ++instI)
                {
                    auto *inst = llvm::dyn_cast<llvm::Instruction>(instI);
                    llvm::ReturnInst *ri = llvm::dyn_cast<llvm::ReturnInst>(inst);
                    if (ri != nullptr)
                    {
                        if (has_exit)
                        {
                            llvm::errs() << "we have multiple return inst in function " << func_i.getName() << "!\n";
                            exit(-1);
                        }
                        has_exit = true;
                        // llvm::errs() << "we have return inst!\n";
                        insert_exit_function_points.push_back(ri);
                        // llvm::errs() << "we have function enter!\n";
                    }
                }
            }

            if (!has_entry)
            {
                llvm::errs() << "we do not have entry in function " << func_i.getName() << "!\n";
                exit(-1);
            }
            if (!has_exit)
            {
                llvm::errs() << "we do not have ret in function " << func_i.getName() << "!\n";
                continue;
            }
            unsigned int rand_num = rand();
            insert_entry_function(insert_entry_function_points, rand_num, m);
            insert_exit_function(insert_exit_function_points, rand_num, m);
        }
    }
};

REGISTER_INSTRUMENTOR(ProcessConcurrentCoverageInstrumentor, new ProcessConcurrentCoverageInstrumentor());