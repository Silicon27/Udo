//
// Created by David Yang on 2025-10-18.
//

#ifndef AST_HPP
#define AST_HPP

#include <support/source_manager.hpp>

namespace udo::ast {
    class Stmt;
    class Decl;
}

namespace udo::ast {
    enum class StmtKind {
        Expression,
        If,
        While,
        For,
        Return,
        Block,
    };

    class Stmt {
    public:
        StmtKind kind;
        
    };

    enum class DeclKind {
        Variable,
        Function,
        Struct,
        Enum,
        Module,
    };

    class Decl {
    public:
        DeclKind kind;

        udo::Source_Range source_range;
    };
}


#endif //AST_HPP
