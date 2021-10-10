set(TARGET ${PROJECT_NAME}-common)

add_library(${TARGET} "")
add_library("SensorityCollector::Common" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/common/Constants.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/common/Logger.hpp"
           "${CMAKE_CURRENT_LIST_DIR}/include/common/Utils.hpp"
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/Utils.cpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(${TARGET} PUBLIC Poco::Foundation)

target_include_directories(${TARGET} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
