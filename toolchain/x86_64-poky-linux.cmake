if(DEFINED ENV{SDK_ROOT})
    set(SDK_ROOT $ENV{SDK_ROOT})
else()
    set(SDK_ROOT "/opt/poky/3.3.1")
endif()

set(TARGET_SYSROOT "${SDK_ROOT}/sysroots/core2-64-poky-linux")
set(NATIVE_SYSROOT "${SDK_ROOT}/sysroots/x86_64-pokysdk-linux")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSROOT ${TARGET_SYSROOT})

set(CMAKE_FIND_ROOT_PATH ${TARGET_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(NATIVE_PREFIX "${NATIVE_SYSROOT}/usr/bin/x86_64-poky-linux/x86_64-poky-linux-")

set(CMAKE_C_COMPILER
    ${NATIVE_PREFIX}gcc
    CACHE PATH "C compiler"
)
set(CMAKE_CXX_COMPILER
    ${NATIVE_PREFIX}g++
    CACHE PATH "C++ compiler"
)
set(CMAKE_LINKER
    ${NATIVE_PREFIX}ld
    CACHE PATH "linker"
)
set(CMAKE_STRIP
    ${NATIVE_PREFIX}strip
    CACHE PATH "strip"
)
set(CMAKE_AR
    ${NATIVE_PREFIX}gcc-ar
    CACHE PATH "archive"
)
set(CMAKE_NM
    ${NATIVE_PREFIX}gcc-nm
    CACHE PATH "nm"
)
set(CMAKE_OBJCOPY
    ${NATIVE_PREFIX}objcopy
    CACHE PATH "objcopy"
)
set(CMAKE_OBJDUMP
    ${NATIVE_PREFIX}objdump
    CACHE PATH "objdump"
)
set(CMAKE_RANLIB
    ${NATIVE_PREFIX}ranlib
    CACHE PATH "ranlib"
)

set(CMAKE_C_FLAGS
    "-m64 -march=core2 -mtune=core2 -msse3 -mfpmath=sse"
    CACHE STRING "" FORCE
)
set(CMAKE_CXX_FLAGS
    "-m64 -march=core2 -mtune=core2 -msse3 -mfpmath=sse"
    CACHE STRING "" FORCE
)

set(ENV{PKG_CONFIG_SYSROOT_DIR} "${TARGET_SYSROOT}")
set(ENV{PKG_CONFIG_PATH}
    "${TARGET_SYSROOT}/usr/lib/pkgconfig:${TARGET_SYSROOT}/usr/share/pkgconfig"
)
