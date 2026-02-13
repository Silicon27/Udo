//
// Created by David Yang on 2025-11-12.
//

#include <ast/ast.hpp>

namespace udo::ast {
    void TranslationUnitDecl::addDecl(Decl *decl) {
        if (!first_decl) {
            first_decl = last_decl = decl;
        } else {
            last_decl->next = decl;
            last_decl = decl;
        }
    }
}
