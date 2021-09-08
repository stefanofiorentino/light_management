cmake_minimum_required(VERSION 3.0.0)
project(light_management VERSION 0.1.0)

include(CTest)
enable_testing()

option(USE_ASAN "Use address sanitizer by adding -fsanitize=address -fno-omit-frame-pointer flags" OFF)
option(USE_UBSAN "Use address sanitizer by adding -fsanitize=undefined -fno-sanitize-recover=all -fno-omit-frame-pointer flags" OFF)

add_executable(light_management main.cpp)
target_compile_features(light_management PUBLIC cxx_std_17)
target_include_directories(light_management PUBLIC ${CMAKE_SOURCE_DIR}/include)


if(USE_ASAN)
    target_compile_options(light_management PUBLIC $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>)
    target_link_libraries(light_management PUBLIC $<$<CONFIG:Debug>:-fsanitize=address>)
endif()

if(USE_UBSAN)
    target_compile_options(light_management PUBLIC $<$<CONFIG:Debug>:-fsanitize=undefined -fno-sanitize-recover=all -fno-omit-frame-pointer>)
    target_link_libraries(light_management PUBLIC $<$<CONFIG:Debug>:-fsanitize=undefined>)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
