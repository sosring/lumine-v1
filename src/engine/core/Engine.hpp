#pragma once

#include <glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Input.hpp"
#include "renderer/Renderer.hpp"
#include "scene/Scene.hpp"
#include "scene/Camera.hpp"

enum class AppState
{
    DebugMode,
    GameMode,
};

class Engine
{
  public:
    Engine(const int width = 1200, const int height = 800);
    ~Engine();

    // Delete copy and move semantics
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    Engine(Engine &&) = delete;
    Engine &operator=(Engine &&) = delete;

    void Run();

  private:
    // --- Lifecycle ---
    void InitScene();
    void InitImGui();
    void Shutdown();

    // --- Per-frame ---
    void PollEvents();
    void Update(float dt);
    void Render();
    void DrawDebugUI();

  private:
    // Window / context
    Window window;
    Input input;
    Renderer renderer;
    Scene scene;

    bool quit = false;
    AppState state = AppState::DebugMode;

    // Timing
    Uint64 lastTicks = 0;
};
