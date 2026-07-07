#pragma once

#include <glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "graphics/Shader.hpp"
#include "graphics/VertexBuffer.hpp"
#include "graphics/IndexBuffer.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/Texture.hpp"

#include <memory>

enum AppState
{
    DebugMode,
    GameMode,
};

class Engine
{
  public:
    Engine(const int width = 800, const int height = 600);
    ~Engine();

    Engine(const Engine &) = delete;            // Delete copy constructor
    Engine &operator=(const Engine &) = delete; // Delete copy assignment

    void Run();

  private:
    // --- Lifecycle ---
    void InitWindow();
    void InitGL();
    void InitScene();
    void InitImGui();
    void Shutdown();

    // --- Per-frame ---
    void PollEvents();
    void Update(float dt);
    void Render();
    void DrawScene();
    void DrawDebugUI();

  private:
    // Window / context
    SDL_Window *window = nullptr;
    SDL_GLContext glContext = nullptr;
    int width, height;
    bool quit = false;

    AppState state = AppState::DebugMode;

    // Timing
    Uint64 lastTicks = 0;

    // Scene / camera
    Camera camera;

    // Resource
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<VertexBuffer> vbo;
    std::unique_ptr<IndexBuffer> ebo;
    std::unique_ptr<Texture> textureBrick;
    std::unique_ptr<Texture> textureCat;
};
