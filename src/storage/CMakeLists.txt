set(TARGET ${PROJECT_NAME}-storage)

add_library(${TARGET} "")
add_library("SensorityCollector::Storage" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/storage/StorageSubsystem.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/DataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/SensorDataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/DataWriter.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/CompositeDataObserver.hpp"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/StorageSubsystem.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/DataObserver.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/SensorDataObserver.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/DataWriter.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/CompositeDataObserver.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(
    ${TARGET} PUBLIC SensorityCollector::Common SensorityCollector::Connectivity Poco::Util
                     Poco::JSON
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
