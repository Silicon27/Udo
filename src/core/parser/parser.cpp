//
// Created by David Yang on 2025-10-18.
//

#include "parser.hpp"

namespace udo::parse {
    Parser::Parser(const std::vector<Token> &tokens, const int allowed_errors, const std::shared_ptr<ProgramNode> &program) : program(program), tokens(tokens), allowed_errors(allowed_errors) {}

}