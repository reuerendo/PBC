# PocketBook SDK Toolchain for CMake

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# SDK paths
set(SDK_ROOT "${CMAKE_SOURCE_DIR}/SDK/SDK_6.3.0/SDK-B288")
set(TOOLCHAIN_ROOT "${SDK_ROOT}")

# Compiler paths
set(CMAKE_C_COMPILER "${TOOLCHAIN_ROOT}/bin/arm-obreey-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_ROOT}/bin/arm-obreey-linux-gnueabi-g++")
set(CMAKE_STRIP "${TOOLCHAIN_ROOT}/bin/arm-obreey-linux-gnueabi-strip")

# Sysroot
set(CMAKE_SYSROOT "${TOOLCHAIN_ROOT}/arm-obreey-linux-gnueabi/sysroot")
set(CMAKE_FIND_ROOT_PATH "${CMAKE_SYSROOT}")

# Include and library paths
set(INKVIEW_INCLUDE_DIR "${CMAKE_SYSROOT}/usr/include")
set(INKVIEW_LIBRARY_DIR "${CMAKE_SYSROOT}/usr/lib")

# Search paths
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -linkview -lfreetype -lz -lpthread")