//
// Created by David Yang on 2025-10-18.
//

#ifndef INSTRUCTION_SELECTOR_HPP
#define INSTRUCTION_SELECTOR_HPP

enum Instruction {
    MOV,
    ADD,
    SUB,
    ADRP,
    LDR,
    STR,
    B,
    BL,
    BDOTLE, // b.le branch if less than or equal to
    BDOTGE, // b.ge branch if greater than or equal to
    BDOTEQ, // b.eq branch if equal
    BDOTNE, // b.ne branch if not equal
    BDOTLT, // b.lt branch if less than
    BDOTGT, // b.gt branch if greater than
};


#endif //INSTRUCTION_SELECTOR_HPP
