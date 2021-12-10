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
endif()

if (SC_CODE_FORMAT)
    include(CodeFormat)
endif()

include(AddPoco)
include(AddMosquitto)
include(AddBoost)
include(AddProtobuf)
include(AddGrpc)

if (DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif()
