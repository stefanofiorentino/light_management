find_library(MULL_LIB mull-ir-frontend)
if(NOT MULL_LIB)
  message(FATAL_ERROR "mull-ir-frontend library not found")
endif()