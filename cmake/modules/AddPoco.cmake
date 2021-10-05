find_package(Poco REQUIRED COMPONENTS Foundation Util Net Data DataSQLite)

if (TARGET Poco::Foundation)
    get_target_property(_LOCATION Poco::Foundation LOCATION)
    message(STATUS "Found Poco[Foundation]: ${_LOCATION}")
endif()

if (TARGET Poco::Util)
    get_target_property(_LOCATION Poco::Util LOCATION)
    message(STATUS "Found Poco[Util]: ${_LOCATION}")
endif()

if (TARGET Poco::Net)
    get_target_property(_LOCATION Poco::Net LOCATION)
    message(STATUS "Found Poco[Net]: ${_LOCATION}")
endif()

if (TARGET Poco::Data)
    get_target_property(_LOCATION Poco::Data LOCATION)
    message(STATUS "Found Poco[Data]: ${_LOCATION}")
endif()

if (TARGET Poco::DataSQLite)
    get_target_property(_LOCATION Poco::DataSQLite LOCATION)
    message(STATUS "Found Poco[DataSQLite]: ${_LOCATION}")
endif()

