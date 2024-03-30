cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(factorygame)

set(CMAKE_CXX_STANDARD 20)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Our Project
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

# Dependencies
# FastNoise
add_subdirectory(deps/FastNoise2)
target_link_libraries(${PROJECT_NAME} FastNoise2)
# Raylib
add_subdirectory(deps/raylib)
target_link_libraries(${PROJECT_NAME} raylib)

# Networking
if (WIN32)
    #    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")

    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif ()

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif ()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()

# Run a client after build
#add_custom_command(TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME} ARGS -client
#        COMMENT "Running ${PROJECT_NAME}")