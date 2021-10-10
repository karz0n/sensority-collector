set(TARGET ${PROJECT_NAME}-storage-test-common)

add_library(${TARGET} "")
add_library("SensorityCollector::Storage::Test::Common" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/storage/MockDataWriter.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/MockDataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/MockDataStorage.hpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(${TARGET} PUBLIC SensorityCollector::Storage SensorityCollector::Tests)

target_include_directories(
    ${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)
