#!/bin/bash

find ./test/ -not -type d -not -name "*.txt" | xargs -r -n1 clang-format-14 --fallback-style=Mozilla -i