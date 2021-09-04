set(TARGET ${PROJECT_NAME}-storage)

add_library(${TARGET} "")
add_library("SensorityCollector::Storage" ALIAS ${TARGET})

target_sources(
        ${TARGET}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/storage/StorageSubsystem.hpp
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/StorageSubsystem.cpp
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(${TARGET} PUBLIC SensorityCollector::Common Poco::Util)

target_include_directories(
        ${TARGET}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> ${PROJECT_OUTPUT_DIR}
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)
