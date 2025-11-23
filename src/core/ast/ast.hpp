//
// Created by David Yang on 2025-10-18.
//

#ifndef AST_HPP
#define AST_HPP

#include <vector>

namespace udo::ast {
    // visitors

    class Visitor {
    public:
        Visitor() = default;
        virtual ~Visitor() = default;

        virtual void visit(std::shared_ptr<class ProgramNode> node);
        virtual void visit(std::shared_ptr<class EvaluationNode> node);
        virtual void visit(std::shared_ptr<class TypeNode> node);
        virtual void visit(std::shared_ptr<class LiteralNode> node);
        virtual void visit(std::shared_ptr<class IdentifierNode> node);
    };

    class WhichNode final : public Visitor {
    public:
        WhichNode() = default;

        void visit(std::shared_ptr<ProgramNode> node) override;
        void visit(std::shared_ptr<EvaluationNode> node) override;
        void visit(std::shared_ptr<TypeNode> node) override;
        void visit(std::shared_ptr<LiteralNode> node) override;
        void visit(std::shared_ptr<IdentifierNode> node) override;

        [[nodiscard]] std::string get_node_name() const;

        std::string node_name;
    };
}

namespace udo::ast {
    class ASTNode : std::enable_shared_from_this<ASTNode> {
        std::shared_ptr<ASTNode> parent;
        std::vector<std::shared_ptr<ASTNode>> children;
    public:
        ASTNode();
        virtual ~ASTNode() = default;

        virtual void accept(Visitor* visitor);

        virtual void add_child(std::shared_ptr<ASTNode> child);
        virtual void remove_child(std::shared_ptr<ASTNode> child);
        virtual void remove_all_children();

        [[nodiscard]] virtual std::shared_ptr<ASTNode> get_child(const std::shared_ptr<ASTNode>&);
        [[nodiscard]] virtual std::vector<std::shared_ptr<ASTNode>>& get_children();
        [[nodiscard]] virtual std::shared_ptr<ASTNode> get_root();

        virtual void add_parent(std::shared_ptr<ASTNode> parent);

    };

    class ExpressionNode : public ASTNode {
    public:
        ExpressionNode() = default;
        ~ExpressionNode() override = default;
    };

    class StatementNode : public ASTNode {
    public:
        StatementNode() = default;
        ~StatementNode() override = default;
    };

    class ProgramNode final : public ASTNode {
    public:
        ProgramNode() = default;
        ~ProgramNode() override = default;
    };

    /// Represents a block of code wrapped in {}
    class BlockNode final : public ASTNode {
        // children member handle things in the block
    public:
        BlockNode() = default;
        ~BlockNode() override = default;
    };

    // used to store expressions
    class EvaluationNode final : public ExpressionNode {
        std::string expression;
    public:
        explicit EvaluationNode(const std::string& expression);
        ~EvaluationNode() override = default;

        [[nodiscard]] const std::string& get_expression() const;
    };

    class TypeNode final : public ExpressionNode {
        std::string type_name;
    public:
        explicit TypeNode(const std::string& name);
        ~TypeNode() override = default;

        [[nodiscard]] const std::string& get_type_name() const;
    };

    // Can be either an unknown token (i.e. type names, names of user defined operations)
    class IdentifierNode final : public ExpressionNode {
        std::string name;
    public:
        explicit IdentifierNode(const std::string &name);
        ~IdentifierNode() override = default;
        [[nodiscard]] const std::string& get_name() const;
    };

    class LiteralNode final : public ExpressionNode {
        std::string value;
    public:
        explicit LiteralNode(const std::string &value);
        ~LiteralNode() override = default;
        [[nodiscard]] const std::string& get_value() const;
    };

    class VariableDeclarationNode final : public ExpressionNode {
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<EvaluationNode> expression;
        std::shared_ptr<TypeNode> type;
    public:
        explicit VariableDeclarationNode(const std::string &name, std::shared_ptr<EvaluationNode> expression, std::shared_ptr<TypeNode> type);
        ~VariableDeclarationNode() override = default;
    };

    class IfStatementNode final : public StatementNode {
        std::shared_ptr<EvaluationNode> condition;


    };
}



#endif //AST_HPP
