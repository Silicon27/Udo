cmake_minimum_required(VERSION 3.31)
project(udo)

set(CMAKE_CXX_STANDARD 20)

# Try to find system LLVM first
find_package(LLVM CONFIG)

if(LLVM_FOUND)
    message(STATUS "Found system LLVM: ${LLVM_DIR}")
else()
    message(STATUS "LLVM not found, fetching/building...")

    include(ExternalProject)

    ExternalProject_Add(
            llvm_project
            GIT_REPOSITORY https://github.com/llvm/llvm-project.git
            GIT_TAG llvmorg-16.0.0  # replace with the LLVM version you want
            PREFIX ${CMAKE_BINARY_DIR}/llvm
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/llvm/install
            -DCMAKE_BUILD_TYPE=Release
    )

    # Make your main target depend on this build
    add_dependencies(udo llvm_project)

    # After build, set LLVM_DIR to the install path for includes/libs
    set(LLVM_DIR ${CMAKE_BINARY_DIR}/llvm/install/lib/cmake/llvm)
    find_package(LLVM REQUIRED CONFIG PATHS ${LLVM_DIR})
endif()

# Map components and link
llvm_map_components_to_libnames(llvm_libs core support)
add_executable(udo
        src/core/cli/main.cpp
        src/core/cli/compiler_invocation.cpp
)
target_link_libraries(udo PRIVATE ${llvm_libs})
target_include_directories(udo PRIVATE ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})