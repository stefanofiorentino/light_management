#!/bin/bash

clang-12 -fexperimental-new-pass-manager \
              -fpass-plugin=/usr/lib/mull-ir-frontend-12 \
              -g -grecord-command-line \
              main.cpp -o hello-world
mull-runner-12 -ide-reporter-show-killed hello-world              