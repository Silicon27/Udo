//
// Created by David Yang on 2025-11-12.
//

#include "ast.hpp"

namespace ast {
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