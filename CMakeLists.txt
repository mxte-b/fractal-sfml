cmake_minimum_required(VERSION 3.28)
project(FractalSFML LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/events.cpp
        src/eventhandler.cpp
        src/inputhandler.cpp
        src/camera.cpp
        src/quaternion.cpp
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)

# Collect shader files
file(GLOB_RECURSE SHADER_FILES
        ${CMAKE_SOURCE_DIR}/shaders/*.frag
        ${CMAKE_SOURCE_DIR}/shaders/*.vert
        ${CMAKE_SOURCE_DIR}/shaders/*.glsl
)

# Create a custom target that copies shaders
add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
        DEPENDS ${SHADER_FILES}
        COMMENT "Copying shader files..."
)

# Make sure it runs before build
add_dependencies(${PROJECT_NAME} copy_shaders)