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
set(ENGINE_CORE_SRC
    src/engine/core/Engine.cpp
)

set(ENGINE_SCENE_SRC
    src/engine/scene/Camera.cpp
    src/engine/scene/Scene.cpp
)

set( ENGINE_RENDERER_SRC
    src/engine/renderer/Renderer.cpp
    src/engine/renderer/Mesh.cpp
    src/engine/renderer/Model.cpp
    src/engine/renderer/Texture.cpp
    src/engine/renderer/Shader.cpp
)

# ---------------- Imgui ----------------

set(IMGUI_SRC
     vendor/imgui/imgui.cpp
     vendor/imgui/imgui_draw.cpp
     vendor/imgui/imgui_impl_opengl3.cpp
     vendor/imgui/imgui_impl_sdl3.cpp
     vendor/imgui/imgui_tables.cpp
     vendor/imgui/imgui_widgets.cpp
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
	src/engine
	src/engine/core
	src/engine/renderer
	sr/engine/scene
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
