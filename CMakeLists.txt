cmake_minimum_required(VERSION 3.10)
project(calibre-companion C)

# Source files
set(SOURCES
    main.c
)

# Create executable
add_executable(calibre-companion.app ${SOURCES})

# Link libraries - inkview автоматически подключает необходимые библиотеки
target_link_libraries(calibre-companion.app
    inkview
)

# Strip executable in release builds
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_custom_command(TARGET calibre-companion.app POST_BUILD
        COMMAND ${CMAKE_STRIP} $<TARGET_FILE:calibre-companion.app>
        COMMENT "Stripping executable"
    )
endif()
