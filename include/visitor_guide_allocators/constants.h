#pragma once

#include <string>

namespace experimental {
namespace constants {
static std::string const& VERY_SHORT_STRING = "short";
static std::string const& VERY_LONG_STRING = "Very looooooooooooooong string";
static std::string const& VERY_LONG_STRING_1 = "Very looooooooooooooong string 1";
static std::string const& VERY_LONG_STRING_2 = "Very looooooooooooooong string 2";
static std::string const& VERY_LONG_STRING_3 = "Very looooooooooooooong string 3";
static char const* const VERY_LONG_STRING_C_STR = VERY_LONG_STRING.c_str();
static std::string const& VERY_LONG_STRING_CAUSING_EXCEPTION = R"({
    "string": "Very looooooooooooooong string, Very looooooooooooooong string, Very looooooooooooooong string"
})";
} // namespace constants
} // namespace experimental
