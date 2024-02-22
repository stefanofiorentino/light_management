add_library(sdk STATIC sdk.c)
target_include_directories(sdk PRIVATE ${CMAKE_SOURCE_DIR}/include)
