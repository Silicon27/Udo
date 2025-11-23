//
// Created by David Yang on 2025-10-20.
//

#include "compiler_invocation.hpp"

#include <utility>
#include <cli/argparse.hpp>

#define CUDO_NAME "cudo"
#define CUDO_VERSION "0.0.0"


using namespace udo;
using namespace udo::lexer;

compiler_config::Compiler_Config parse(const int argc, char *argv[])  {
    compiler_config::Compiler_Config c;
    argparse::ArgumentParser program(CUDO_NAME,
        CUDO_VERSION, argparse::default_arguments::all, false);

    program.add_argument("input")
        .help("The input file to compile")
        .required();

    // optional, defaults to non-.udo-postfixed version of input file
    program.add_argument("output")
        .help("output file (defaults to binary)");

    program.add_argument("-v", "--verbose")
        .help("Print verbose output");

    program.add_argument("-O0")
        .help("(attempt to) Disable all optimizations");

    program.add_argument("-O1")
        .help("Light optimizations, default");

    program.add_argument("-O2")
        .help("Heavy optimizations");

    program.add_argument("-O3")
        .help("Aggro mode!");

    program.add_argument("-Os")
        .help("Optimize for size");

    program.add_argument("-Oz")
        .help("Optimize for speed");

    // TODO make distinct arguments for each output format


    program.add_argument("--fmax-error-count")
        .help("The maximum number of errors recovery attempts before compilation termination")
        .nargs(1, 1);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
    }

    auto input = program.get<std::string>("input");
    auto output = program.get<std::string>("output");
    int fmax_error_count = program.get<int>("--fmax-error-count");

    compiler_config::Compiler_Config config;

    if (output.empty()) {
        // TODO alter this depending on whether user wants the .o files or the linked binary
        // TODO MAYBE REMOVE IN FAVOR OF EXPLICIT -o
        std::string temp = input.substr(0, input.find_last_of('.'));
        config.output = temp;
    } else {
        config.output = output;
    }

    config.sources.push_back(input);
    config.flags.max_error_count = fmax_error_count;

    return config;
}

// constructor for Compiler_Invocation
Compiler_Invocation::Compiler_Invocation(compiler_config::Compiler_Config config)  : config(std::move(config)) {}

// Preprocessor Invoke



// Lexer_Invoke
compiler_config::Lexer_Invoke::Lexer_Invoke(const Param &param) : param(param) {}
std::unique_ptr<Lexer> compiler_config::Lexer_Invoke::invoke() const {
    return std::make_unique<Lexer>(param.input_Stream);
}


