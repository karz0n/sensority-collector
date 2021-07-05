cmake_minimum_required(VERSION 3.8)

# -------------------------------------------------------------------------------
# Project Declaration
# -------------------------------------------------------------------------------

project(sensority-collector)

# -------------------------------------------------------------------------------
# Project Configuration
# -------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectOptions.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfigs.cmake)

# -------------------------------------------------------------------------------
# Structure Configuration
# -------------------------------------------------------------------------------

add_subdirectory(src)
if(SC_ENABLE_TESTS)
    add_subdirectory(test)
endif()
