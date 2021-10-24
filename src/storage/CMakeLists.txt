set(TARGET ${PROJECT_NAME}-storage)

add_library(${TARGET} "")
add_library("SensorityCollector::Storage" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/storage/StorageSubsystem.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/DataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/SensorDataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/DataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/CompositeDataObserver.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/DataStorage.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/IDataStorage.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/DataTypes.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/IDataJob.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/IDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/TypedDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/eBreathVocDataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/eCo2DataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/GasDataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/HumidityDataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/IaqDataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/PressureDataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/TempDataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/accessor/TvocDataAccessor.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/TempDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/eCo2DataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/eBreathVocDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/GasDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/HumidityDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/IaqDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/PressureDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/TempDataModel.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/storage/model/TvocDataModel.hpp"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/StorageSubsystem.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/DataObserver.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/SensorDataObserver.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/CompositeDataObserver.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/DataStorage.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/TypedDataModel.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/DataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/eBreathVocDataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/eCo2DataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/GasDataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/HumidityDataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/IaqDataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/PressureDataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/TempDataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/accessor/TvocDataAccessor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/model/eCo2DataModel.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/model/eBreathVocDataModel.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/model/GasDataModel.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/model/HumidityDataModel.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/model/IaqDataModel.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/model/PressureDataModel.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/model/TempDataModel.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/src/model/TvocDataModel.cpp"
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
