set(TARGET ${PROJECT_NAME}-common)

add_library(${TARGET} INTERFACE "")
add_library("SensorityCollector::Common" ALIAS ${TARGET})

target_sources(
    ${TARGET}
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include/common/Logger.hpp"
)

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET})

target_link_libraries(${TARGET} INTERFACE Poco::Foundation)

target_include_directories(
    ${TARGET}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> ${PROJECT_OUTPUT_DIR}
)
