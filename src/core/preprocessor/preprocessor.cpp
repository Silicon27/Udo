//
// Created by David Yang on 2025-10-18.
//

#include "preprocessor.hpp"

Preprocessor::Preprocessor(std::string file) : file(file, std::ios::in) {

}



std::string Preprocessor::get_next_line_with_preprocessing_directive() {
    file.open();
    while (file.getline)
}
