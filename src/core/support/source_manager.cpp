//
// Created by David Yang on 2025-11-30.
//

#include <support/source_manager.hpp>

namespace udo {
    static SourceLoc make_source_loc(const FileID file, const Offset offset) { return (file << 32) | offset; }
    static FileID loc_to_FileID(const SourceLoc loc) { return loc >> 32; }
    static Offset loc_to_Offset(const SourceLoc loc) { return loc & 0xFFFFFFFFu; }


    std::string Source_Manager::fetch_line() {
        std::string line;
        std::getline(source_output_stream, line);
        return line;

    }

}