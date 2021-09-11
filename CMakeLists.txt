cmake_minimum_required(VERSION 3.8)

# -------------------------------------------------------------------------------
# Project Declaration
# -------------------------------------------------------------------------------

project(
    sensority-collector
    VERSION 0.1
    DESCRIPTION "Sensority collector service"
)

# -------------------------------------------------------------------------------
# Project Configuration
# -------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectOptions.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfigs.cmake)

# -------------------------------------------------------------------------------
# Structure Configuration
# -------------------------------------------------------------------------------

add_subdirectory(src)
add_subdirectory(tools)
