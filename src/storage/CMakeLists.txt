set(TARGET ${PROJECT_NAME}-storage)

add_library(${TARGET} "")
add_library("SensorityCollector::Storage" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/storage/StorageSubsystem.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/DataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/SensorDataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/IDataWriter.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/IDataReader.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/IDataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/CompositeDataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/IDataStorage.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/DataStorage.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/Co2Data.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/EvocData.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/GasData.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/HumidityData.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/IaqData.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/PressureData.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/TemperatureData.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/TvocData.hpp"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/StorageSubsystem.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/DataObserver.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/SensorDataObserver.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/CompositeDataObserver.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/DataStorage.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/Co2Data.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/EvocData.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/GasData.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/HumidityData.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/IaqData.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/PressureData.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/TemperatureData.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/TvocData.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(
    ${TARGET} PUBLIC SensorityCollector::Common SensorityCollector::Connectivity Poco::Util
                     Poco::JSON Poco::Data Poco::DataSQLite
)

target_include_directories(
    ${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)

if(SC_ENABLE_TESTS)
    add_subdirectory(test/common)
    add_subdirectory(test/unit)
endif()
