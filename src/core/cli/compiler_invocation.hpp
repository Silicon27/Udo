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

namespace compiler_config {
    struct CompilerConfig {
        std::vector<std::string> sources;
        std::unordered_map<std::string, std::string> flags;
        std::optional<std::string> output;
    };

    CompilerConfig parse(int argc, char *argv[]);

    /// @brief Individual invocation for the Lexer and it's objects
    ///
    /// @param param param data for Lexer constructor
    struct Lexer_Invoke {
        /// @brief calling parameters for invocation of the Lexer object
        /// @param inputStream a `std::istream` object representing the input file
        struct Param {
            std::istream &inputStream;
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

        };
    };
}


class Compiler_Invocation {
    compiler_config::CompilerConfig config;
public:
    explicit Compiler_Invocation(const compiler_config::CompilerConfig& config);


};

#endif //COMPILER_INVOCATION_HPP
