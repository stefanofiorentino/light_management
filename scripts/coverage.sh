#!/bin/bash

TMP_NAME=$(mktemp -d)

cd ${TMP_NAME} \
    && cmake \
        -DCMAKE_BUILD_TYPE:STRING=Debug \
        -DFETCH_3RD_PARTY_REPOS:BOOL=TRUE \
        -DUSE_TEST_COVERAGE:BOOL=TRUE \
        ${OLDPWD} \
    && make \
        -j$(nproc) \
    && ctest \
        --timeout 30 \
        -T Test \
        -T Coverage \
        -C Debug \
        -j2 \
        --output-on-failure \
        2>&1

cd ${OLDPWD} && rm -r ${TMP_NAME}
