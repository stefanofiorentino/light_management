#!/bin/bash

mull-runner-12 \
    --ld-search-path=/usr/lib/x86_64-linux-gnu \
    --strict \
    $1 \
    2>&1 | \
    ( ! grep "Survived:" )