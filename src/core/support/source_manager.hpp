//
// Created by David Yang on 2025-11-30.
//

#ifndef SOURCE_MANAGER_HPP
#define SOURCE_MANAGER_HPP
#include <string>
#include <ostream>
#include <fstream>

namespace udo {

    // forward types
    using FileID = uint32_t;
    using Offset = uint32_t;
    using SourceLoc = uint64_t; // packed (FileID << 32) | offset

    static SourceLoc make_source_loc(FileID file, Offset offset);
    static FileID loc_to_FileID(SourceLoc loc);
    static Offset loc_to_Offset(SourceLoc loc);



}

#endif //SOURCE_MANAGER_HPP
