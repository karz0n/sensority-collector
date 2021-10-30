include(GNUInstallDirs)

if(NOT SC_SYSTEMD_UNITS)
    set(SC_SYSTEMD_UNITS "${CMAKE_INSTALL_LIBDIR}/systemd/system" CACHE FILEPATH "Systemd units path" FORCE)
endif()
message(VERBOSE "Systemd units path: ${SC_SYSTEMD_UNITS}")

if(NOT SC_SYSTEMD_USER)
    set(SC_SYSTEMD_USER "root" CACHE STRING "User to run service as" FORCE)
endif()
message(VERBOSE "Systemd user: ${SC_SYSTEMD_USER}")

if(NOT SC_SYSTEMD_GROUP)
    set(SC_SYSTEMD_GROUP "root" CACHE STRING "Group to run service as" FORCE)
endif()
message(VERBOSE "Systemd group: ${SC_SYSTEMD_GROUP}")

configure_file(scollector.service.in scollector.service @ONLY)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/scollector.service
    DESTINATION ${SC_SYSTEMD_UNITS}
    COMPONENT SensorityCollectorRuntime
)
