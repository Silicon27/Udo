//
// Created by David Yang on 2025-10-18.
//

#include "parser.hpp"

namespace udo::parse {
    Token Parser::peek(const int n) { return tokens[pos + n]; }

    Parser::Parser(const std::vector<Token> &tokens, const Flags &flag, std::shared_ptr<ProgramNode> &program)
    : program(program), tokens(tokens), flags(flag) {}


}