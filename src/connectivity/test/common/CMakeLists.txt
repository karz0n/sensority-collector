set(TARGET ${PROJECT_NAME}-connectivity-test-common)

add_library(${TARGET} "")
add_library("SensorityCollector::Connectivity::Test::Common" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/connectivity/MockMqttClient.hpp"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/MockMqttClient.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(${TARGET} PUBLIC SensorityCollector::Storage GTest::gtest GTest::gmock)

target_include_directories(
    ${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)
