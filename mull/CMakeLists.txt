if (NOT "${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/clang++-12")
    message(WARNING "skipping mull, not managed without clang++-12 (CXX=${CMAKE_CXX_COMPILER})")    
    return()
endif()

find_file(MULL_LIB mull-ir-frontend-12 PATHS /usr/lib /usr/local/lib)
if(NOT MULL_LIB)
    message(WARNING "mull-ir-frontend library not found")
    return()
endif()    

find_program(MULL_RUNNER mull-runner-12 PATHS /usr/bin /usr/local/bin)
if(NOT MULL_LIB)
    message(WARNING "mull-runner-12 not found")
    return()
endif()

add_executable(
    mull 
    main.cpp
)
target_compile_options(
    mull 
        PUBLIC 
    -fexperimental-new-pass-manager
    -fpass-plugin=/usr/lib/mull-ir-frontend-12
)
add_custom_command(
    TARGET mull POST_BUILD
    COMMAND mull-runner-12 -ide-reporter-show-killed $<TARGET_FILE:mull>
    VERBATIM
)