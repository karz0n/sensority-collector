set(TARGET ${PROJECT_NAME}-connectivity)

add_library(${TARGET} "")
add_library("SensorityCollector::Connectivity" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/connectivity/ConnectivitySubsystem.hpp"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/ConnectivitySubsystem.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(${TARGET} PUBLIC PkgConfig::Mosquitto Poco::Util)

target_include_directories(
    ${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> ${PROJECT_OUTPUT_DIR}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)
