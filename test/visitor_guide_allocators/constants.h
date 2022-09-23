#pragma once

#include <string>

namespace experimental {
namespace constants {
static std::string const &VERY_LONG_STRING = "Very looooooooooooooong string";
static char const *const VERY_LONG_STRING_C_STR = VERY_LONG_STRING.c_str();
static std::string const &VERY_LONG_STRING_CAUSING_EXCEPTION = R"({
    "string": "Very looooooooooooooong string, Very looooooooooooooong string, Very looooooooooooooong string"
})";
} // namespace constants
} // namespace experimental
