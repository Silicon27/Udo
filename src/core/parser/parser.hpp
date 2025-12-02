//
// Created by David Yang on 2025-10-18.
//

#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include <ast/ast.hpp>
#include <lexer/lexer.hpp>
#include <error/error.hpp>

#include <cli/compiler_invocation.hpp>

using namespace udo::compiler_config;
namespace udo::parse {

    struct Flags {
        // frontend flags
        bool verbose = false;
        int max_error_count = 20;

        // backend flags
        Opt_Level     level        = Opt_Level::O1;
        Output_Format output_format = Output_Format::Executable;
        std::string   output_file;        // raw -o argument, if any

        // pipeline control
        bool          link = true;       // whether to invoke the linker / produce final executable
    };

    class Parse;

    using namespace udo::ast;
    using namespace udo::lexer;

    struct ParserSnapshot {
        int capped_pos;

    };

    class Parser {
        std::shared_ptr<ProgramNode> program;
        std::vector<Token> tokens;
        Flags flags;
        int pos = 0;

    public:
        Token peek(int n = 0);

        explicit Parser(const std::vector<Token> &tokens, const Flags &flag, std::shared_ptr<ProgramNode> &program);
        ~Parser() = default;

        void parse();

    };
}

#endif //PARSER_HPP
