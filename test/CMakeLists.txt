#-------------------------------------------------------------------------------

set(TARGET_NAME ${PROJECT_NAME}-test)

#-------------------------------------------------------------------------------
# Target Configuration
#-------------------------------------------------------------------------------

add_executable(${TARGET_NAME} "")

target_sources(${TARGET_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/test.cpp
    )

target_link_libraries(${TARGET_NAME}
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
    )