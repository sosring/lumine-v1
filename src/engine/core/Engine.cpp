#include "Engine.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

Engine::Engine(int width, int height) : window("Lumine-v1-gl", width, height)
{
    InitScene();
    InitImGui();

    lastTicks = SDL_GetTicks();
};

Engine::~Engine()
{
    Shutdown();
}

void Engine::InitScene()
{
    scene.Load();
};

void Engine::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    // (void)io;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.Alpha = 0.8f;

    // Initialize backends
    ImGui_ImplSDL3_InitForOpenGL(window.Handle(), window.GLContext());
    ImGui_ImplOpenGL3_Init("#version 410");
};

void Engine::DrawDebugUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");

    // Will use this for editing light color and intensity
    if (ImGui::CollapsingHeader("Engine", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool wireframe = renderer.Wireframe();
        ImGui::ColorPicker4("Light Color", glm::value_ptr(scene.LightColor()));
        if (ImGui::Checkbox("Draw Geometry", &wireframe))
            renderer.SetWireframe(wireframe);

        ImGui::DragFloat3("Light Position", glm::value_ptr(scene.LightPosition()));
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Render()
{
    renderer.Render(scene, camera, window);

    if (state == AppState::DebugMode)
        DrawDebugUI();

    window.SwapBuffers();
};

void Engine::Update(float dt)
{
    // Camera Keyboard Movement
    if (input.IsKeyDown(SDL_SCANCODE_W))
        camera.ProcessKeyboardMovement(dt, Direction::Forward);
    if (input.IsKeyDown(SDL_SCANCODE_S))
        camera.ProcessKeyboardMovement(dt, Direction::BackWard);
    if (input.IsKeyDown(SDL_SCANCODE_A))
        camera.ProcessKeyboardMovement(dt, Direction::Left);
    if (input.IsKeyDown(SDL_SCANCODE_D))
        camera.ProcessKeyboardMovement(dt, Direction::Right);
    if (input.IsKeyDown(SDL_SCANCODE_SPACE))
        camera.ProcessKeyboardMovement(dt, Direction::Up);
    if (input.IsKeyDown(SDL_SCANCODE_LSHIFT))
        camera.ProcessKeyboardMovement(dt, Direction::Down);

    if (state == AppState::GameMode)
        camera.ProcessMouseMovement(input.MouseDeltaX(), input.MouseDeltaY());
};

void Engine::Run()
{
    while (!quit)
    {
        // DeltaTime
        Uint64 currentTicks = SDL_GetTicks();
        float dt = (currentTicks - lastTicks) / 1000.0f;

        PollEvents();
        Update(dt);
        Render();

        lastTicks = currentTicks;
    }
}

void Engine::PollEvents()
{
    input.BeginFrame();

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL3_ProcessEvent(&e);
        input.ProcessEvent(e);

        switch (e.type)
        {
            case SDL_EVENT_KEY_DOWN:
                if (e.key.key == SDLK_Q)
                {
                    state = state == AppState::DebugMode ? AppState::GameMode : AppState::DebugMode;
                    window.SetRelativeMouseMode(state == AppState::GameMode);
                }
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                window.Resize(e.window.data1, e.window.data2);
                break;
            default:
                break;
        }
    }

    if (input.QuitRequested())
        quit = true;
}

void Engine::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
};
