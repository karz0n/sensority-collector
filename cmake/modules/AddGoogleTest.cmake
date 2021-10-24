include(GoogleTest)

if(DEFINED ENV{GTEST_ROOT})
    list(APPEND CMAKE_PREFIX_PATH $ENV{GTEST_ROOT})
endif()

find_package(GTest CONFIG REQUIRED)

if(TARGET GTest::gtest)
    get_property(
        GTEST_LOCATION
        TARGET GTest::gtest
        PROPERTY LOCATION
    )
    message(STATUS "Found GTest: ${GTEST_LOCATION}")
else()
    message(FATAL_ERROR "GTest library NOT found")
endif()

if(TARGET GTest::gmock)
    get_property(
        GMOCK_LOCATION
        TARGET GTest::gmock
        PROPERTY LOCATION
    )
    message(STATUS "Found GMock: ${GMOCK_LOCATION}")
else()
    message(FATAL_ERROR "GMock library NOT found")
endif()

function(gtest_enable)
    set(prefix ARG)
    set(noValues DEFAULT_LIBS DEFAULT_MAIN)
    set(singleValues TARGET DIRECTORY)
    set(multiValues "")

    cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}" "${multiValues}" ${ARGN})

    if(NOT DEFINED ARG_TARGET)
        message(FATAL_ERROR "The name of the target is mandatory argument")
    endif()

    if (ARG_DEFAULT_LIBS)
        list(APPEND ARG_LIBS GTest::gtest GTest::gmock)
    endif()
    if(ARG_DEFAULT_MAIN)
        list(APPEND ARG_LIBS GTest::gtest_main GTest::gmock_main)
    endif()
    if (DEFINED ARG_LIBS)
        target_link_libraries(${ARG_TARGET} PRIVATE ${ARG_LIBS})
    endif()

    if (NOT CMAKE_CROSSCOMPILING)
        gtest_discover_tests(${TARGET} WORKING_DIRECTORY ${PROJECT_OUTPUT_DIR})
    endif()
endfunction()
