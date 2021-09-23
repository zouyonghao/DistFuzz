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
#define CLANG "clang"
#endif

inline bool is_assembly_file(int argc, char **argv)
{
    string last_arg = argv[argc - 1];
    if (last_arg.rfind(".s") == last_arg.length() - 2 || last_arg.rfind(".S") == last_arg.length() - 2)
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
    std::string clang_exe = CLANG;
    if (clang_exe.find("++") == std::string::npos)
    {
        // compile for C
        if (std::getenv("CLANG_VERSION") != NULL)
        {
            clang_exe = std::getenv("CLANG_VERSION");
            std::cout << "use " << clang_exe << "\n";
        }
    }
    else
    {
        // compile for C++
        if (std::getenv("CLANG++_VERSION") != NULL)
        {
            clang_exe = std::getenv("CLANG++_VERSION");
        }
    }
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
        instrumentors.push_back(Registry<Instrumentor>::getItemMap()[std::string(i.GetString())]);
    }

    std::vector<std::string> file_blacklist;
    for (auto &i : document["file_blacklist"].GetArray())
    {
        file_blacklist.push_back(i.GetString());
    }

    if (is_assembly_file(argc, argv))
    {
        execvp(clang_exe.c_str(), argv);
        perror((std::string("failed to exec ") + clang_exe).c_str());
        exit(-1);
    }

    std::vector<const char *> only_link_vector{clang_exe.c_str()};
    std::vector<const char *> compile_to_ll_vector{clang_exe.c_str(), "-S", "-emit-llvm"};

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
    bool has_black_list_file = false;
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
        else if (ends_with(tmp, ".c") || ends_with(tmp, ".cpp") || ends_with(tmp, ".cc") || ends_with(tmp, ".cxx"))
        {
            std::string arg_string = std::string(argv[i]);
            for (auto &black_list_file : file_blacklist)
            {
                if (arg_string.find(black_list_file) != std::string::npos)
                {
                    has_black_list_file = true;
                }
            }
            source_files.push_back(arg_string);
            is_only_link = false;
        }

        if (is_should_ignored(tmp, document["ignored_args"]))
        {
            continue;
        }

        only_link_vector.push_back(argv[i]);
        compile_to_ll_vector.push_back(argv[i]);
    }

    if (is_only_link || has_black_list_file)
    {
        for (auto &i : document["additional_link_args"].GetArray())
        {
            only_link_vector.push_back(i.GetString());
        }
        only_link_vector.push_back(nullptr);
        execvp(only_link_vector[0], (char *const *)&only_link_vector[0]);
        perror((std::string("failed to exec ") + clang_exe).c_str());
        exit(-1);
    }

    if (is_only_compile)
    {
        // emit llvm
        if (fork() == 0)
        {
            compile_to_ll_vector.push_back(nullptr);
            execvp(compile_to_ll_vector[0], (char *const *)&compile_to_ll_vector[0]);
            perror("Failed to compile sources to .ll files.");
            exit(-1);
        }
        wait(NULL);

        std::vector<std::string> ll_files;
        // instrument
        for (auto &s : source_files)
        {
            std::string ll_file_name = s.substr(s.rfind("/") + 1, s.rfind(".") - s.rfind("/") - 1) + ".ll";
            ll_files.push_back(ll_file_name);

            for (auto &i : document["instrumentors"].GetArray())
            {
                auto map = Registry<Instrumentor>::getItemMap();
                if (map.find(i.GetString()) != map.end())
                {
                    llvm::LLVMContext ctx;
                    llvm::SMDiagnostic Err;
                    unique_ptr<llvm::Module> module_ptr = llvm::parseIRFile(ll_file_name, Err, ctx);
                    if (!module_ptr)
                    {
                        llvm::errs() << "ParseIRFile failed\n";
                        llvm::errs() << Err.getMessage() << "\n";
                        return -EBADF;
                    }
                    map[i.GetString()]->do_instrument(module_ptr.get());

                    std::error_code ec;
                    llvm::raw_ostream *out = new llvm::raw_fd_ostream(llvm::StringRef(ll_file_name), ec);
                    module_ptr->print(*out, nullptr);
                }
            }
        }

        std::vector<const char *> compile_ll_to_target_vector{clang_exe.c_str(), "-c", "-fPIC", "-O0", "-g"};
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
        for (auto &i : document["additional_compile_llvm_args"].GetArray())
        {
            compile_ll_to_target_vector.push_back(i.GetString());
        }
        compile_ll_to_target_vector.push_back(nullptr);
        execvp(compile_ll_to_target_vector[0], (char *const *)&compile_ll_to_target_vector[0]);
        perror("Failed to compile .ll files to target.");
        exit(-1);
    }

    // TODO
    ERROR_LOG("Currently we do not support to run compling and linking together.\n");

    for (auto &i : document["additional_link_args"].GetArray())
    {
        only_link_vector.push_back(i.GetString());
    }
    only_link_vector.push_back(nullptr);
    execvp(only_link_vector[0], (char *const *)&only_link_vector[0]);
    perror((std::string("failed to exec ") + clang_exe.c_str()).c_str());
    exit(-1);

    return 0;
}
