//
// Created by David Yang on 2025-10-18.
//

#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include "../ast/ast.hpp"
#include "../lexer/lexer.hpp"

namespace udo::parse {
    class Parse;

    using namespace udo::ast;
    using namespace udo::lexer;

    class Parser {
        std::shared_ptr<ProgramNode> program;
        std::vector<Token> tokens;
        int allowed_errors;
    public:
        explicit Parser(const std::vector<Token> &tokens, int allowed_errors = 20, std::shared_ptr<ProgramNode> program);
        ~Parser() = default;


    };
}

#endif //PARSER_HPP
