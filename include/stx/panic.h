/**
 * @file panic.h
 * @author Basit Ayantunde <rlamarrr@gmail.com>
 * @brief
 * @version  0.1
 * @date 2020-04-26
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "stx/report.h"

#ifdef STX_STABLE_SOURCE_LOCATION
#include <source_location>
#else
#include <experimental/source_location>
#endif

// TODO(lamarrr): Add halting, aborting, and other widely used panic behaviors

namespace stx {

#ifdef STX_STABLE_SOURCE_LOCATION
using SourceLocation = std::source_location;
#else
using SourceLocation = std::experimental::source_location;
#endif

// here, we can avoid any form of memory allocation that might be needed,
// therefore deferring the info string and report payload to the callee and can
// also use a stack allocated string in cases where dynamic memory allocation is
// undesired the debugging breakpoint should be attached to this function
[[noreturn]] void panic_handler(std::string_view info,
                                ReportPayload const& payload,
                                SourceLocation location);

/// This allows a program to terminate immediately and provide feedback to the
/// caller of the program. `panic` should be used when a program reaches an
/// unrecoverable state. This function is the perfect way to assert conditions
/// in example code and in tests. `panic` is closely tied with the `unwrap` and
/// `expect` method of both `Option` and `Result`. Both implementations call
/// `panic` when they are set to `None` or `Err` variants.
[[noreturn]] STX_FORCE_INLINE void panic(
    std::string_view info,
    SourceLocation location = SourceLocation::current()) {
  panic_handler(std::move(info), ReportPayload(Report("")),
                std::move(location));
}

[[noreturn]] STX_FORCE_INLINE void panic(
    std::string_view info, Reportable auto const& value,
    SourceLocation location = SourceLocation::current()) {
  panic_handler(std::move(info),
                ReportPayload(internal::report::query >> value),
                std::move(location));
}

[[noreturn]] STX_FORCE_INLINE void panic(
    std::string_view info, auto const& value,
    SourceLocation location = SourceLocation::current()) {
  (void)value;
  panic_handler(std::move(info), ReportPayload(Report("")),
                std::move(location));
}

};  // namespace stx
