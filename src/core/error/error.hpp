//
// Created by David Yang on 2025-11-13.
//

#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>

namespace udo::diag {
    // Helper types

    /// Indexes a location in the source file
    struct Source_Location {
        int line;
        int column;
        std::string file;
    };

    /// ranks severity of the diagnostic at hand, may be demoted/promoted during parsing/sema even after initialization
    enum class Severity {
        Ignore,     // Do not present this diagnostic, ignore it

        Note,       // Reminder to user, no detected defiance of compiler rules

        Warning,    // Detected uses that defy rules, still compilable.

        Error,      // Compile time: cannot be handled and indicates an unrecoverable state (syntax errors, unknown symbol errors, etc.)
                    // Runtime: Unless handled, will terminate the program, can be user-defined, caught, and handled.

        Fatal       // Immediately terminates the program, cannot be handled, indicates an unrecoverable
                    // state from rule breaks (kernel level disrupts, etc)
    };

    /// The proposed replacement for compliant, non-optimal code
    struct Fixit {
        Source_Location location;
        std::string replacement;
    };

    struct Diagnostic {
        Source_Location location;
        Severity severity;
        std::string message;
        std::vector<Fixit> fixits;
    };

}

namespace udo::diag {

    // Udo diagnostics library
    //  - 2025 David Yang
    // Udo diagnostics module and error handler

    class Diagnostics_Engine {
    public:
        Diagnostics_Engine() = default;
        ~Diagnostics_Engine() = default;
    };
}

#endif //ERROR_HPP
