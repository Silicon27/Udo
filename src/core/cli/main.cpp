#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "compiler_invocation.hpp"

/// cudo - Udo Compiler

int main(int argc, char* argv[]) {
    using namespace udo;

    Compiler_Invocation ci(compiler_config::parse(argc, argv));

}
