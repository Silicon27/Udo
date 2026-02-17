//
// Created by David Yang on 2025-10-18.
//

#ifndef AST_HPP
#define AST_HPP

#include <support/source_manager.hpp>
#include <type_traits>

namespace udo::ast {
    class Type;
    class QualType;
    class Decl;
    class Stmt;
    class Expr;
    class ASTContext;

    class Type {
        friend class ASTContext;
    public:
        enum class Kind {
            Builtin,
            UserDefined,
        };

    protected:
        explicit Type(const Kind K) : typeKind(K) {}
        [[nodiscard]] Kind getKind() const { return typeKind; }
    private:
        Kind typeKind;
    };
    static_assert(std::is_trivially_destructible_v<Type>);

    class BuiltinType : public Type {
    public:
        enum class BuiltinKind {
            I4,
            I8,
            I16,
            I32,
            I64,
            I128,
            F4,
            F8,
            F16,
            F32,
            F64,
            F128,
            Char,
            Bool,
        };
    protected:
        explicit BuiltinType(const BuiltinKind BK)
            : Type(Kind::Builtin), builtinKind(BK) {}

        [[nodiscard]] BuiltinKind getBuiltinKind() const { return builtinKind; }
    private:
        BuiltinKind builtinKind;
    };
    static_assert(std::is_trivially_destructible_v<BuiltinType>);

    /// A base class for any declaration that can contain other declarations.
    class DeclContext {
        Decl* first_decl = nullptr;
        Decl* last_decl = nullptr;

    protected:
        DeclContext() = default;

    public:
        void addDecl(Decl* decl);

        [[nodiscard]] Decl* getFirstDecl() const { return first_decl; }
        [[nodiscard]] Decl* getLastDecl() const { return last_decl; }
    };
    static_assert(std::is_trivially_destructible_v<DeclContext>);

    /// Base class for all declarations.
    class Decl {
        friend class ASTContext;
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
    static_assert(std::is_trivially_destructible_v<Decl>);

    /// The top-level declaration that represents the entire translation unit.
    class TranslationUnitDecl : public Decl, public DeclContext {
    public:
        TranslationUnitDecl()
            : Decl(Kind::TranslationUnit) {}
    };
    static_assert(std::is_trivially_destructible_v<TranslationUnitDecl>);

    /// Base class for all statements.
    class Stmt {
        friend class ASTContext;
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
    static_assert(std::is_trivially_destructible_v<Stmt>);

    /// For statement chaining
    class CompoundStmt final : public Stmt {
        std::uint32_t num_stmts = 0;

        explicit CompoundStmt(std::uint32_t num_stmts)
            : Stmt(Kind::CompoundStmt), num_stmts(num_stmts) {}

    public:
        static CompoundStmt* Create(ASTContext& C, Stmt** stmts, std::uint32_t num_stmts);

        using iterator = Stmt**;
        using const_iterator = Stmt* const*;

        [[nodiscard]] std::uint32_t size() const { return num_stmts; }
        [[nodiscard]] Stmt** get_stmts() { return reinterpret_cast<Stmt**>(this + 1); }
        [[nodiscard]] Stmt* const* get_stmts() const { return reinterpret_cast<Stmt* const*>(this + 1); }

        [[nodiscard]] iterator begin() { return get_stmts(); }
        [[nodiscard]] iterator end() { return get_stmts() + num_stmts; }

        [[nodiscard]] const_iterator begin() const { return get_stmts(); }
        [[nodiscard]] const_iterator end() const { return get_stmts() + num_stmts; }
    };
    static_assert(std::is_trivially_destructible_v<CompoundStmt>);

    /// Base class for all expressions, which are also statements.
    class Expr : public Stmt {
    protected:
        explicit Expr(const Kind K) : Stmt(K) {}
    };
    static_assert(std::is_trivially_destructible_v<Expr>);
}

#endif //AST_HPP
