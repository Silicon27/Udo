//
// Created by David Yang on 2025-10-20.
//

#ifndef COMPILER_INVOCATION_HPP
#define COMPILER_INVOCATION_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include "../preprocessor/preprocessor.hpp"
#include "../lexer/lexer.hpp"
#include "../ast/ast.hpp"
#include "../parser/parser.hpp"

namespace udo::compiler_config {


    using namespace udo;
    using namespace udo::lexer;

    enum class Opt_Level {
        O0, // compiler attempts 1:1 of source code, minimal change in output (disabling as many opts as possible)
        O1, // refer to https://llvm.org/doxygen/classllvm_1_1OptimizationLevel.html for all the below.
        O2,
        O3,
        Os,
        Oz
    };

    enum class Output_Format {
        /// Native object file (ELF / Mach-O / COFF depending on target)
        Object,

        /// Assembly (.s)
        Assembly,

        /// LLVM IR (textual .ll)
        LLVM_IR,

        /// LLVM Bitcode (.bc)
        LLVM_Bitcode,

        /// wasm object file (WebAssembly)
        Wasm_Object,

        /// wasm text format (.wat)
        Wasm_Text,

        /// Null output (used for benchmarking / analysis)
        Null,

        // Native executable, requires lld
        Executable
    };

    struct Flags {
        // frontend flags
        bool verbose = false;
        int max_error_count = 20;

        // backend flags
        Opt_Level level = Opt_Level::O1;
        Output_Format output_format;
    };

    /// Necessary arguments
    struct Compiler_Config {
        std::vector<std::string> sources; // sources to compile from
        Flags flags;
        std::optional<std::string> output;
    };

    // is meant to take the output of the argc++ library, and is not meant to parse the arguments itself.
    Compiler_Config parse(int argc, char *argv[]);

    struct Preprocessor_Invoke {
        struct Param {
            std::string input_file;
        };
        Param param;
        explicit Preprocessor_Invoke(const Param &param);
        Preprocessor preprocessor;
    };

    /// @brief Individual invocation for the Lexer and it's objects
    ///
    /// @param param param data for Lexer constructor
    struct Lexer_Invoke {
        /// @brief calling parameters for invocation of the Lexer object
        /// @param inputStream a `std::istream` object representing the input file
        struct Param {
            std::istream &input_Stream;
        };

        /// Mutable because Param.inputStream may be altered by Lexer constructor
        mutable Param param;

        explicit Lexer_Invoke(const Param &param);

        /// @brief Initialize a Lexer object with Lexer constructor params
        /// @returns Lexer object,
        std::unique_ptr<Lexer> invoke() const;
    };

    /// @brief Individual invocation of Parser and it's objects
    struct Parser_Invoke {
        struct Param {
            std::shared_ptr<ast::ProgramNode> program;
            std::vector<Token> tokens;
            int allowed_errors;
        };

        mutable Param param;

        explicit Parser_Invoke(const Param &param);

        std::unique_ptr<parse::Parser> invoke() const;
    };

    struct Linker_Invoke {

    };
}


class Compiler_Invocation {
    udo::compiler_config::Compiler_Config config;
public:
    explicit Compiler_Invocation(udo::compiler_config::Compiler_Config config);
};

#endif //COMPILER_INVOCATION_HPP
