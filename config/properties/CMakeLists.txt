include(GNUInstallDirs)

if (NOT SC_DATABASE_PATH)
    set(SC_DATABASE_PATH "${CMAKE_INSTALL_FULL_DATADIR}/sensority/scollector.db"
        CACHE FILEPATH "Database file path" FORCE)
endif()
message(VERBOSE "Database file path: ${SC_DATABASE_PATH}")

configure_file(scollector.properties.in scollector.properties @ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/scollector.properties
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT SensorityCollectorRuntime
)