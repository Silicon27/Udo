//
// Created by David Yang on 2025-10-20.
//

#include "compiler_invocation.hpp"


compiler_config::CompilerConfig parse(const int argc, char *argv[])  {
   compiler_config::CompilerConfig c;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-o" && i + 1 < argc) c.output = argv[++i];
        else if (a.starts_with("-")) c.flags[a] = "true";
        else c.sources.push_back(a);
    }
    return c;
}

// constructor for Compiler_Invocation
Compiler_Invocation::Compiler_Invocation(const compiler_config::CompilerConfig &config)  : config(config) {}


// Lexer_Invoke
compiler_config::Lexer_Invoke::Lexer_Invoke(const Param &param) : param(param) {}
std::unique_ptr<Lexer> compiler_config::Lexer_Invoke::invoke() const {
    return std::make_unique<Lexer>(param.inputStream);
}


