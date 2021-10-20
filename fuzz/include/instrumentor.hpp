#ifndef __INSTRUMENTOR_HEADER__
#define __INSTRUMENTOR_HEADER__

#include <dst_registry.hpp>

#include <vector>

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Module.h>

/**
 *
 * This class is a abstract class of instrumentors.
 * So what a instrumentor really needs?
 *      Name
 *      The location to instrument
 *      The function to be instrumented
 *
 */
class Instrumentor
{
private:
public:
    Instrumentor() {}
    ~Instrumentor() {}

    virtual std::vector<llvm::Instruction *> get_locations(llvm::Module *) = 0;

    virtual llvm::Function *get_function(llvm::Module *) = 0;

    virtual void do_instrument(llvm::Module *) = 0;
};

#define REGISTER_INSTRUMENTOR(name, item) bool name##_entry = Registry<Instrumentor>::add(#name, item)

#endif // __INSTRUMENTOR_HEADER__
