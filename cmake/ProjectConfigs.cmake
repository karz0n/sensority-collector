list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify build location and build type
include(BuildLocation)
include(BuildType)

# Enable tests support
if(SC_ENABLE_TESTS)
    enable_testing()
    include(AddGoogleTest)
    message(STATUS "Building with tests")
else()
    message(STATUS "Building without tests")
endif()

include(AddPoco)
include(AddMosquitto)
include(AddBoost)
