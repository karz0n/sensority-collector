include(GNUInstallDirs)

install(
    FILES scollector.properties
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT SensorityCollectorRuntime
)