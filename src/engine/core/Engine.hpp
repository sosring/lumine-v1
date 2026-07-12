#pragma once

#include <glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/scene/Camera.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Model.hpp"

#include <memory>

enum class AppState
{
    DebugMode,
    GameMode,
};

class Engine
{
  public:
    Engine(const int width = 800, const int height = 600);
    ~Engine();

    // Delete copy and move semantics
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    Engine(Engine &&) = delete;
    Engine &operator=(Engine &&) = delete;

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

    // Test Variable
    bool drawGeometry = false;
    bool spinModel = false;

    AppState state = AppState::DebugMode;

    // Timing
    Uint64 lastTicks = 0;

    // Scene / camera
    Camera camera;

    // Resource
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Model> backpack;
};
