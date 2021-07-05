list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

# Specify build location and build type
include(BuildLocation)
include(BuildType)

# Check and specify C++17 standard
include(CxxTestCompiler)
check_for_cxx17(CXX17_COMPILER)
if(CXX17_COMPILER)
    enable_cxx17()
else()
    message(FATAL_ERROR "The C++17 standard not supported")
endif()

# Enable tests support
if(SC_ENABLE_TESTS)
    enable_testing()
    include(AddGoogleTest)
    message(STATUS "Building with tests")
else()
    message(STATUS "Building without tests")
endif()
