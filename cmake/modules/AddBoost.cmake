find_package(Boost REQUIRED)

if(DEFINED Boost_FOUND)
    message(STATUS "Found Boost: ${Boost_VERSION_STRING}")
endif()