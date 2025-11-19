cmake_minimum_required(VERSION 3.10)
project(calibre-companion C)

# Include directories
include_directories(${INKVIEW_INCLUDE_DIR})

# Link directories
link_directories(${INKVIEW_LIBRARY_DIR})

# Source files
set(SOURCES
    main.c
)

# Create executable
add_executable(calibre-companion.app ${SOURCES})

# Link libraries
target_link_libraries(calibre-companion.app
    inkview
    freetype
    z
    pthread
)

# Strip executable in release builds
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_custom_command(TARGET calibre-companion.app POST_BUILD
        COMMAND ${CMAKE_STRIP} $<TARGET_FILE:calibre-companion.app>
        COMMENT "Stripping executable"
    )
endif()