set(TARGET ${PROJECT_NAME}-storage-test)

add_executable(${TARGET} "")

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src/SensorDataObserverTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/CompositeDataObserverTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/accessor/eBreathVocDataAccessorTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/accessor/eCo2DataAccessorTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/accessor/GasDataAccessorTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/accessor/HumidityDataAccessorTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/accessor/IaqDataAccessorTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/accessor/PressureDataAccessorTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/accessor/TempDataAccessorTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/accessor/TvocDataAccessorTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/model/eCo2DataModelTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/model/eBreathVocDataModelTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/model/GasDataModelTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/model/HumidityDataModelTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/model/IaqDataModelTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/model/PressureDataModelTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/model/TvocDataModelTest.cpp"
           "${CMAKE_CURRENT_LIST_DIR}/src/model/TempDataModelTest.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(
    ${TARGET}
    PUBLIC SensorityCollector::Storage SensorityCollector::Storage::Test::Common
           SensorityCollector::Connectivity::Test::Common Tools::Tests
)

target_include_directories(
    ${TARGET}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)

gtest_enable(TARGET ${TARGET} DIRECTORY ${PROJECT_OUTPUT_DIR})
