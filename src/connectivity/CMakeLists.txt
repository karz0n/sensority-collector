set(TARGET ${PROJECT_NAME}-connectivity)

add_library(${TARGET} "")
add_library("SensorityCollector::Connectivity" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/connectivity/ConnectivitySubsystem.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/connectivity/MqttCommon.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/connectivity/IMqttClient.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/connectivity/MqttClient.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/connectivity/MqttInitializer.hpp"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/ConnectivitySubsystem.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/MqttCommon.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/MqttClient.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/MqttInitializer.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(
    ${TARGET} PUBLIC Boost::headers PkgConfig::Mosquitto Poco::Foundation Poco::Util
                     SensorityCollector::Common
)

target_include_directories(
    ${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> ${PROJECT_OUTPUT_DIR}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)

if(SC_ENABLE_TESTS)
    add_subdirectory(test/common)
    add_subdirectory(test/unit)
endif()
