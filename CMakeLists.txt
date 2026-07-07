cmake_minimum_required(VERSION 3.16)

project(lumine-v1 VERSION 0.01)

find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
	# -------- Engine Source ------------- 
        src/main.cpp
        src/engine/Engine.cpp

	# -------- glad/stb ------------- 
        vendor/glad/glad.c
        vendor/stb/stb_image.c

	# -------- ImGui ------------- 
        vendor/imgui/imgui.cpp
        # vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_impl_sdl3.cpp
        vendor/imgui/imgui_impl_opengl3.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    vendor/glad
    vendor/imgui
    vendor/stb
    src
)

target_link_libraries(${PROJECT_NAME}
    SDL3::SDL3
    glm::glm
    OpenGL::GL
)

# Copy assets
add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_SOURCE_DIR}/res
        ${CMAKE_BINARY_DIR}/res
)

add_dependencies(${PROJECT_NAME} copy_resources)
