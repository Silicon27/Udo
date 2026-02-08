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
    class Stmt {
    public:
        enum class StmtKind {
            Expression,
            If,
            While,
            For,
            Return,
            Block,
        };
    };

    class Decl {
    public:
        enum class DeclKind {
            Variable,
            Function,
            Struct,
            Enum,
            Module,
        };

        udo::Source_Range source_range;
    };
}


#endif //AST_HPP
