//
// Created by David Yang on 2025-10-18.
//

#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <fstream>

class Preprocessor {
    std::fstream file;
public:
    explicit Preprocessor(std::string file);
    ~Preprocessor();

    /// we read line by line, as long as the line does not start
    /// with @ we skip, otherwise return the line
    std::string get_next_line_with_preprocessing_directive();

    void preprocess();
};

#endif //PREPROCESSOR_HPP
