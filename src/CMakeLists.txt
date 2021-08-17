set(TARGET ${PROJECT_NAME})

add_executable(${TARGET} "")

target_sources(
    ${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/SensorityApplication.hpp"
                      "${CMAKE_CURRENT_LIST_DIR}/SensorityApplication.cpp"
)

target_link_libraries(
    ${TARGET} Poco::Util SensorityCollector::Storage SensorityCollector::Connectivity
)

add_subdirectory(connectivity)
add_subdirectory(storage)
