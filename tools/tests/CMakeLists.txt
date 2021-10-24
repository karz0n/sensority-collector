set(TARGET ${PROJECT_NAME}-tests)

add_library(${TARGET} "")
add_library("Tools::Tests" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/tests/Waiter.hpp"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/TestApplication.hpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/TestApplication.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(${TARGET} PUBLIC Poco::Util GTest::gtest GTest::gmock)

target_include_directories(
    ${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)
