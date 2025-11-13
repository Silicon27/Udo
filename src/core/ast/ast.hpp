//
// Created by David Yang on 2025-10-18.
//

#ifndef AST_HPP
#define AST_HPP

#include <vector>

namespace ast {
    class Visitor {
        Visitor() = default;
        virtual ~Visitor() = default;

        virtual void visit(class ProgramNode* node);
    };

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

        [[nodiscard]] virtual std::shared_ptr<ASTNode> get_child(ASTNode);
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

    class EvaluationNode final : public ExpressionNode {
    public:
        EvaluationNode() = default;
        ~EvaluationNode() override = default;
    };

    class IdentifierNode final : public ExpressionNode {
        std::string name;
    public:
        IdentifierNode() = default;
        ~IdentifierNode() override = default;
    };
}



#endif //AST_HPP
