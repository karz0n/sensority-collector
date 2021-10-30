include(GNUInstallDirs)

set(TARGET ${PROJECT_NAME})

add_executable(${TARGET} "")

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "scollector")

target_sources(
    ${TARGET} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/SensorityApplication.hpp"
                      "${CMAKE_CURRENT_LIST_DIR}/SensorityApplication.cpp"
)

target_link_libraries(
    ${TARGET} Poco::Util SensorityCollector::Storage SensorityCollector::Connectivity
)

install(
    TARGETS ${TARGET}
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT SensorityCollectorRuntime
)

add_subdirectory(common)
add_subdirectory(connectivity)
add_subdirectory(storage)
