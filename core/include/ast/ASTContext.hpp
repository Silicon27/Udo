//
// Created by David Yang on 2026-02-08.
//

#ifndef UDO_AST_CONTEXT_HPP
#define UDO_AST_CONTEXT_HPP

#include <vector>

class ASTContext {
    struct BumpPtrAllocator {
        // Nodes are allocated in a bump pointer allocator for fast allocation and deallocation.
        // This is a simple implementation that does not support deallocation of individual nodes.
        std::vector<char*> blocks;


    };
};

#endif //UDO_AST_CONTEXT_HPP