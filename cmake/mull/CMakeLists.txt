
if(NOT CI_MODE STREQUAL "MUTATION")
    return()
endif()

if (NOT "${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/clang++-12")
    message(FATAL_ERROR "skipping mull, not managed without clang++-12 (CXX=${CMAKE_CXX_COMPILER})")
endif()

find_file(MULL_LIB mull-ir-frontend-12 PATHS /usr/lib /usr/local/lib)
if(NOT MULL_LIB)
    message(FATAL_ERROR "mull-ir-frontend library not found")
endif()

find_program(MULL_RUNNER_12 mull-runner-12)
if(NOT MULL_RUNNER_12)
    message(FATAL_ERROR "mull-runner-12 not found!")
endif()
