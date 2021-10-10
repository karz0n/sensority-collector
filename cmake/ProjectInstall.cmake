include(GNUInstallDirs)

if(SC_ENABLE_SYSTEMD)
    add_subdirectory(${PROJECT_SOURCE_DIR}/systemd)
endif()
