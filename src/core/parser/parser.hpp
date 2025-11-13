//
// Created by David Yang on 2025-10-18.
//

#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include "../ast/ast.hpp"
#include "../lexer/lexer.hpp"

class Parser {
    std::vector<Token> tokens;
    int allowed_errors;
public:
    Parser(std::vector<Token> tokens, int allowed_errors = 20, std::shared_ptr<ProgramNode> program) : tokens(tokens), allowed_errors(allowed_errors) {}
};

#endif //PARSER_HPP
