#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "compiler_invocation.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " <file> [options]\n";
        return 1;
    }

    Compiler_Invocation ci(compiler_config::parse(argc, argv));

}
