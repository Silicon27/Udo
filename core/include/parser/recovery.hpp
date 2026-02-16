//
// Created by David Yang on 2026-02-15.
//

#ifndef UDO_RECOVERY_HPP
#define UDO_RECOVERY_HPP

namespace udo::parse {
    class Recovery;

    // handles common errors and attempts to recover from them to continue parsing and
    // report more errors in one go, instead of just bailing out at the first error.
    class Recovery {
    public:
        enum class ErrType {
            missing_token, // in which case we instead expect the token after the token that is missing instead
        };
        // we first specify common error scenarios and their corresponding recovery strategies,
        // then we implement the logic to detect those scenarios and apply the strategies.

        /// Error recovery strategies for different error scenarios, e.g. if we expect a token
        /// but it's missing, we can skip to the next token and continue parsing from there, or
        /// if we encounter an unexpected token, we can try to find the next valid token and
        /// continue parsing from there.
        struct Strat {

        };

        struct ErrScenario {
            ErrType type;

        };
    };
}

#endif //UDO_RECOVERY_HPP