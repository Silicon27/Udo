#ifndef COMPILER_INVOCATION_HPP
#define COMPILER_INVOCATION_HPP

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <istream>

#include "../preprocessor/preprocessor.hpp"
#include "../lexer/lexer.hpp"
#include "../ast/ast.hpp"
#include "../parser/parser.hpp"

namespace udo::compiler_config {

    using namespace udo;
    using namespace udo::ast;
    using namespace udo::lexer;
    using namespace udo::parse;

    enum class Opt_Level {
        O0, // compiler attempts 1:1 of source code, minimal change in output
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

    /// Necessary arguments
    struct Compiler_Config {
        std::vector<std::string> sources; // sources to compile from
        Flags flags;
        // Resolved output artifact (final exe or single file output).
        // May be std::nullopt for per-source outputs (e.g. -c with multiple files).
        std::optional<std::string> output;
    };

    // takes argv and returns a fully-populated Compiler_Config
    Compiler_Config parse(int argc, char *argv[]);

    struct Preprocessor_Invoke {
        struct Param {
            std::string input_file;
        };

        Param param;
        explicit Preprocessor_Invoke(Param param);

        /// Invoke the preprocessor and return its result.
        /// For now this just returns a Preprocessor instance.
        Preprocessor invoke() const;
    };

    /// @brief Individual invocation for the Lexer and it's objects
    struct Lexer_Invoke {
        /// @brief calling parameters for invocation of the Lexer object
        struct Param {
            std::istream &input_Stream;
        };

        /// Mutable because Param.inputStream may be altered by Lexer constructor
        mutable Param param;

        explicit Lexer_Invoke(const Param &param);

        /// @brief Initialize a Lexer object with Lexer constructor params
        /// @returns Lexer object
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

        explicit Parser_Invoke(Param param);

        std::unique_ptr<parse::Parser> invoke() const;
    };

    struct Sema_Invoke {
        struct Param {
            std::shared_ptr<ast::ProgramNode> program;
            int allowed_errors;
        };

        mutable Param param;

        explicit Sema_Invoke(Param param);

        /// Run semantic analysis, mutate the AST in-place.
        /// For now this is a no-op stub.
        void invoke() const;
    };

    struct Linker_Invoke {
        struct Param {
            Compiler_Config &config;
            std::vector<std::string> object_files; // input object files from all sources
        };

        mutable Param param;

        explicit Linker_Invoke(Param param);

        /// Invoke the linker. For now this is a stub.
        void invoke() const;
    };

} // namespace udo::compiler_config


class Compiler_Invocation {
    udo::compiler_config::Compiler_Config config;

public:
    explicit Compiler_Invocation(const udo::compiler_config::Compiler_Config& config);

    /// Run the entire pipeline (preprocess, lex, parse, sema, codegen, link).
    /// Currently, codegen is not wired here; focus is on CC as orchestration.
    int run();
};

#endif //COMPILER_INVOCATION_HPP