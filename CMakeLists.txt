cmake_minimum_required(VERSION 3.16)

project(lumine-v1-gl VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

# ---------------- Source Files ----------------
file(GLOB ENGINE_CORE_SRC
    CONFIGURE_DEPENDS
    src/engine/core/*.cpp
)

file(GLOB ENGINE_SCENE_SRC
    CONFIGURE_DEPENDS
    src/engine/scene/*.cpp
)

file(GLOB ENGINE_RENDERER_SRC
    CONFIGURE_DEPENDS
    src/engine/renderer/*.cpp
)

file(GLOB IMGUI_SRC
    CONFIGURE_DEPENDS
    vendor/imgui/*.cpp
)

# ---------------- Executable ----------------
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp

        ${ENGINE_CORE_SRC}
        ${ENGINE_SCENE_SRC}
        ${ENGINE_RENDERER_SRC}

        vendor/glad/glad.c
        vendor/stb/stb_image.c

        ${IMGUI_SRC}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        vendor/glad
        vendor/imgui
        vendor/stb
        src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
        glm::glm
        OpenGL::GL
        assimp::assimp
)

# ---------------- Resources ----------------
add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/res
        ${CMAKE_BINARY_DIR}/res
)

add_dependencies(${PROJECT_NAME} copy_resources)
