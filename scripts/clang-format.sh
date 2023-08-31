#!/bin/bash

find . \( -iname "*.h" -o -iname "*.hpp" -o -iname "*.cpp" \) | xargs -r -n1 -I '{}' clang-format-10 --fallback-style=Mozilla -i {}