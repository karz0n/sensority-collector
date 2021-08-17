find_package(PkgConfig REQUIRED)

pkg_check_modules(Mosquitto REQUIRED IMPORTED_TARGET "libmosquittopp >= 2.0.10")