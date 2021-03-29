/**
 *
 * A customized compiler to compile, instrument and link C/C++ source file
 * with user-defined instrumentors and libraries
 *
 */

#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <instrumentor.hpp>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <log.hpp>

using namespace std;

#ifndef CLANG
#ifdef __cplusplus
#define CLANG "clang++"
#else
#define CLANG "clang"
#endif
#endif

inline bool is_assembly_file(int argc, char **argv)
{
    string last_arg = argv[argc - 1];
    if (last_arg.rfind(".s") == last_arg.length() - 2 ||
        last_arg.rfind(".S") == last_arg.length() - 2)
    {
        return true;
    }
    return false;
}

// target name should end with .o and not .mod.o
// inline bool is_object_name(std::string &target_name)
// {
//     return target_name.rfind(".o") == target_name.length() - 2 &&
//            (target_name.length() < 6 ||
//             target_name.rfind(".mod.o") != target_name.length() - 6);
// }

inline bool ends_with(std::string const &value, std::string const &ending)
{
    if (ending.size() > value.size())
    {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

inline bool is_should_ignored(std::string &tmp, rapidjson::Value &ignored_args)
{
    for (auto &i : ignored_args.GetArray())
    {
        if (i.GetString() == tmp)
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv)
{
    std::ifstream ifs("/tmp/compiler-config.json");
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document document;
    if (document.ParseStream(isw).HasParseError())
    {
        std::cout << "Parse /tmp/compiler-config failed!\n";
        std::cout << document.GetParseError() << "\n";
        return -EBADF;
    }
    ifs.close();

    std::vector<Instrumentor *> instrumentors;
    for (auto &i : document["instrumentors"].GetArray())
    {
        instrumentors.push_back(
            Registry<Instrumentor>::getItemMap()[std::string(i.GetString())]);
    }

    if (is_assembly_file(argc, argv))
    {
        execvp(CLANG, argv);
        perror((std::string("failed to exec ") + CLANG).c_str());
        exit(-1);
    }

    std::vector<const char *> only_link_vector{CLANG};
    std::vector<const char *> compile_to_ll_vector{CLANG, "-S", "-emit-llvm"};

    for (auto &i : document["additional_compile_args"].GetArray())
    {
        only_link_vector.push_back(i.GetString());
        compile_to_ll_vector.push_back(i.GetString());
    }
    std::vector<std::string> source_files;

    std::string target_name = "a.out";

    /**
     * conditions: (test.c)
     *      gcc -c test.c -o test.o // only compile
     *      gcc test.c -o test      // compile and link
     *      gcc test.c -o test.o    // compile and link
     *      gcc test.c              // compile and link
     *      gcc test.o -o test      // only link
     */
    bool is_only_link = true;
    bool is_only_compile = false;
    bool is_specified_output_name = false;
    for (int i = 1; i < argc; i++)
    {
        std::string tmp(argv[i]);
        if (tmp == "-o")
        {
            is_specified_output_name = true;
            only_link_vector.push_back(argv[i]);
            if (i + 1 == argc)
            {
                perror("There should be a name after -o");
                exit(-1);
            }
            target_name = std::string(argv[i + 1]);
            only_link_vector.push_back(argv[i + 1]);
            i++;
            continue;
        }
        else if (tmp == "-c")
        {
            is_only_compile = true;
        }
        else if (ends_with(tmp, ".c") || ends_with(tmp, ".cpp") ||
                 ends_with(tmp, ".cc"))
        {
            source_files.push_back(std::string(argv[i]));
            is_only_link = false;
        }

        if (is_should_ignored(tmp, document["ignored_args"]))
        {
            continue;
        }

        only_link_vector.push_back(argv[i]);
        compile_to_ll_vector.push_back(argv[i]);
    }

    if (is_only_link)
    {
        for (auto &i : document["additional_link_args"].GetArray())
        {
            only_link_vector.push_back(i.GetString());
        }
        only_link_vector.push_back(nullptr);
        execvp(only_link_vector[0], (char *const *)&only_link_vector[0]);
        perror((std::string("failed to exec ") + CLANG).c_str());
        exit(-1);
    }

    if (is_only_compile)
    {
        // emit llvm
        if (fork() == 0)
        {
            compile_to_ll_vector.push_back(nullptr);
            execvp(compile_to_ll_vector[0],
                   (char *const *)&compile_to_ll_vector[0]);
            perror("Failed to compile sources to .ll files.");
            exit(-1);
        }
        wait(NULL);

        std::vector<std::string> ll_files;
        // instrument
        for (auto &i : document["instrumentors"].GetArray())
        {
            auto map = Registry<Instrumentor>::getItemMap();
            if (map.find(i.GetString()) != map.end())
            {
                for (auto &s : source_files)
                {
                    std::string ll_file_name =
                        s.substr(s.rfind("/") + 1,
                                 s.rfind(".") - s.rfind("/") - 1) +
                        ".ll";
                    ll_files.push_back(ll_file_name);
                    llvm::LLVMContext ctx;
                    llvm::SMDiagnostic Err;
                    unique_ptr<llvm::Module> module_ptr =
                        llvm::parseIRFile(ll_file_name, Err, ctx);
                    if (!module_ptr)
                    {
                        llvm::errs() << "ParseIRFile failed\n";
                        llvm::errs() << Err.getMessage() << "\n";
                        return -EBADF;
                    }
                    map[i.GetString()]->do_instrument(module_ptr.get());

                    std::error_code ec;
                    llvm::raw_ostream *out =
                        new llvm::raw_fd_ostream(llvm::StringRef(ll_file_name),
                                                 ec, llvm::sys::fs::F_None);
                    module_ptr->print(*out, nullptr);
                }
            }
        }

        std::vector<const char *> compile_ll_to_target_vector{
            CLANG, "-c", "-fPIC", "-O0", "-g"};
        for (auto &i : ll_files)
        {
            compile_ll_to_target_vector.push_back(i.c_str());
        }
        // compile ll to target
        if (is_specified_output_name)
        {
            compile_ll_to_target_vector.push_back("-o");
            compile_ll_to_target_vector.push_back(target_name.c_str());
        }

        for (auto &i : compile_ll_to_target_vector)
        {
            std::cout << i << " ";
        }

        compile_ll_to_target_vector.push_back(nullptr);
        execvp(compile_ll_to_target_vector[0],
               (char *const *)&compile_ll_to_target_vector[0]);
        perror("Failed to compile .ll files to target.");
        exit(-1);
    }

    // TODO
    ERROR_LOG(
        "Currently we do not support to run compling and linking together.\n");
    only_link_vector.push_back(nullptr);
    execvp(only_link_vector[0], (char *const *)&only_link_vector[0]);
    perror((std::string("failed to exec ") + CLANG).c_str());
    exit(-1);

    return 0;
}
