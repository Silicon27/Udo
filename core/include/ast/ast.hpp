//
// Created by David Yang on 2025-10-18.
//

#ifndef AST_HPP
#define AST_HPP

#include <support/source_manager.hpp>

namespace udo::ast {
    class Decl;
    class Stmt;
    class Expr;
    class ASTContext;

    /// Base class for all declarations.
    class Decl {
    public:
        enum class Kind : std::uint8_t {
            TranslationUnit,
            Variable,
            Function,
            Struct,
            Enum,
            Module,
        };

    private:
        Kind declKind;

    protected:
        explicit Decl(const Kind K) : declKind(K) {}

    public:
        ~Decl() = default;
        [[nodiscard]] Kind getKind() const { return declKind; }

        Decl* next = nullptr;

        udo::Source_Range source_range;
    };

    /// The top-level declaration that represents the entire translation unit.
    class TranslationUnitDecl : public Decl {
        Decl* first_decl = nullptr;
        Decl* last_decl = nullptr;
    public:
        TranslationUnitDecl()
            : Decl(Kind::Module) {}

        void addDecl(Decl* decl);

        [[nodiscard]] Decl* getFirstDecl() const { return first_decl; }
        [[nodiscard]] Decl* getLastDecl() const { return last_decl; }
    };

    /// Base class for all statements.
    class Stmt {
    public:
        enum class Kind : std::uint8_t {
            CompoundStmt,
            IfStmt,
            WhileStmt,
            ForStmt,
            ReturnStmt,
            ExprStmt,
        };

    private:
        Kind stmtKind;

    protected:
        explicit Stmt(const Kind K) : stmtKind(K) {}

    public:
        ~Stmt() = default;
        [[nodiscard]] Kind getKind() const { return stmtKind; }
    };

    /// Base class for all expressions, which are also statements.
    class Expr : public Stmt {
    protected:
        explicit Expr(const Kind K) : Stmt(K) {}
    };
}

#endif //AST_HPP
