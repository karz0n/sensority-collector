set(TARGET ${PROJECT_NAME}-storage-test)

add_executable(${TARGET} "")

target_sources(
    ${TARGET} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src/DataObserverTest.cpp"
                     "${CMAKE_CURRENT_LIST_DIR}/src/DataWriterTest.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(
    ${TARGET}
    PUBLIC SensorityCollector::Storage SensorityCollector::Storage::Test::Common
           SensorityCollector::Connectivity::Test::Common SensorityCollector::Tools::Tests
)

target_include_directories(
    ${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)
