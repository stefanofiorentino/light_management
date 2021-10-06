
#
# test based on googletest
#

option(FETCH_3RD_PARTY_REPOS "Fetch 3rd repos" OFF)
include(FetchContent)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if (FETCH_3RD_PARTY_REPOS)
    FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG master
        GIT_SHALLOW 1
    )

    FetchContent_GetProperties(entt)

    if(NOT entt_POPULATED)
        FetchContent_Populate(entt)
    endif()

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
        GIT_SHALLOW 1
    )

    FetchContent_GetProperties(googletest)

    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        
    endif()
else(FETCH_3RD_PARTY_REPOS)
    set(entt_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rd/entt-src")
    set(googletest_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rd/googletest-src")
    set(googletest_BINARY_DIR "${CMAKE_BUILD_DIR}/_deps/googletest-build")
endif(FETCH_3RD_PARTY_REPOS)
add_subdirectory("${googletest_SOURCE_DIR}" "${googletest_BINARY_DIR}" EXCLUDE_FROM_ALL)

add_library(GTest::Main ALIAS gtest_main)
add_library(GMock::Main ALIAS gmock_main)

target_compile_features(gtest PUBLIC cxx_std_17)
target_compile_features(gtest_main PUBLIC cxx_std_17)
target_compile_features(gmock PUBLIC cxx_std_17)
target_compile_features(gmock_main PUBLIC cxx_std_17)

option(USE_ASAN "Use address sanitizer by adding -fsanitize=address -fno-omit-frame-pointer flags" OFF)
option(USE_UBSAN "Use address sanitizer by adding -fsanitize=undefined -fno-sanitize-recover=all -fno-omit-frame-pointer flags" OFF)

function(ADD_UNIT_TEST TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} "${TEST_SOURCE}")

    set_target_properties(${TEST_NAME} PROPERTIES POSITION_INDEPENDENT_CODE 1)
    target_link_libraries(
            ${TEST_NAME}
        PRIVATE
            GMock::Main
            Threads::Threads
    )
    target_include_directories(
        ${TEST_NAME}
            PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        "${googletest_SOURCE_DIR}/googlemock/include"
        "${entt_SOURCE_DIR}/src"
    )
    add_test(NAME gtest_${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)

    if(USE_ASAN)
        target_compile_options(${TEST_NAME} PUBLIC -fsanitize=address -fno-omit-frame-pointer)
        target_link_libraries(${TEST_NAME} PUBLIC -fsanitize=address)
    endif()

    if(USE_UBSAN)
        target_compile_options(${TEST_NAME} PUBLIC -fsanitize=undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
        target_link_libraries(${TEST_NAME} PUBLIC -fsanitize=undefined)
    endif()
endfunction()

option(BUILD_GOOGLE_BASED_TESTS "Enable building googletest-based unit tests." ON)
if (${BUILD_GOOGLE_BASED_TESTS})
    ADD_UNIT_TEST(runtime_polymorphism runtime_polymorphism.cpp)
    ADD_UNIT_TEST(brown_bag_tdd_demo brown_bag_tdd_demo.cpp)
    ADD_UNIT_TEST(std_variant std_variant.cpp)
    ADD_UNIT_TEST(entt entt.cpp)
    ADD_UNIT_TEST(brown_bag_mocking_strategies brown_bag_mocking_strategies.cpp)
    ADD_UNIT_TEST(lights_collection lights_collection.cpp)
    ADD_UNIT_TEST(integral_type integral_type.cpp)
    ADD_UNIT_TEST(pragmatic_type_erasure pragmatic_type_erasure.cpp)
endif (${BUILD_GOOGLE_BASED_TESTS})
