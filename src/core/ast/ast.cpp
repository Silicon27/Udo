//
// Created by David Yang on 2025-11-12.
//

#include "ast.hpp"

namespace udo::ast {
    // Visitor::visit implementations

    void Visitor::visit(const std::shared_ptr<ProgramNode> node) { (void)node; }
    void Visitor::visit(const std::shared_ptr<EvaluationNode> node) { (void)node; }
    void Visitor::visit(const std::shared_ptr<TypeNode> node) { (void)node; }
    void Visitor::visit(const std::shared_ptr<LiteralNode> node) { (void)node; }
    void Visitor::visit(const std::shared_ptr<IdentifierNode> node) { (void)node; }

    // WhichNode::visit
    void WhichNode::visit(const std::shared_ptr<ProgramNode> node) {
        (void)node;
        this->node_name = "ProgramNode";
    }
    void WhichNode::visit(const std::shared_ptr<EvaluationNode> node) {
        (void)node;
        this->node_name = "EvaluationNode";
    }

    void WhichNode::visit(const std::shared_ptr<TypeNode> node) {
        (void)node;
        this->node_name = "TypeNode";
    }

    void WhichNode::visit(const std::shared_ptr<LiteralNode> node) {
        (void)node;
        this->node_name = "LiteralNode";
    }

    void WhichNode::visit(const std::shared_ptr<IdentifierNode> node) {
        (void)node;
        this->node_name = "IdentifierNode";
    }

    std::string WhichNode::get_node_name() const { return this->node_name; }

}

namespace udo::ast {
    ASTNode::ASTNode() : parent(nullptr)  {}

    void ASTNode::accept(Visitor* visitor) {
        (void)visitor;
    }

    void ASTNode::add_child(const std::shared_ptr<ASTNode> child) {
        children.push_back(child);
        child->add_parent(shared_from_this());
    }

    void ASTNode::add_parent(const std::shared_ptr<ASTNode> parent) {
        this->parent = parent;
        add_child(parent);
    }

    void ASTNode::remove_child(const std::shared_ptr<ASTNode> child) {
        std::erase(children, child);
        child->add_parent(nullptr);
    }

    void ASTNode::remove_all_children() {
        for (auto child : children) {
            child->add_parent(nullptr);
        }
        children.clear();
    }


    std::shared_ptr<ASTNode> ASTNode::get_child(const ASTNode c) {
        const auto it = std::ranges::find_if(children, [&](const std::shared_ptr<ASTNode>& child) {
            return *child == c;
        });
        return (it != children.end()) ? *it : nullptr;
    }

    std::vector<std::shared_ptr<ASTNode>>& ASTNode::get_children()  {
        return children;
    }

    std::shared_ptr<ASTNode> ASTNode::get_root() {
        return parent;
    }

}

namespace udo::ast {
    TypeNode::TypeNode(const std::string& name) : type_name(name) {}
    const std::string& TypeNode::get_type_name() const { return type_name; }

    EvaluationNode::EvaluationNode(const std::string &expression) : expression(expression) {}
    const std::string& EvaluationNode::get_expression() const { return expression; }

    LiteralNode::LiteralNode(const std::string &value) : value(value) {}
    const std::string &LiteralNode::get_value() const { return value; }

    IdentifierNode::IdentifierNode(const std::string &name) : name(name) {}
    const std::string &IdentifierNode::get_name() const { return name; }



}