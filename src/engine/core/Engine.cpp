#include "Engine.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>
#include <memory>

Engine::Engine(int width, int height) : window("Lumine-v1-gl", width, height)
{
    InitScene();
    InitImGui();
    window.SetRelativeMouseMode(state == AppState::GameMode);

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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Debug");

    if (ImGui::CollapsingHeader("Engine", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PushID("Engine");

        bool wireframe = renderer.Wireframe();
        if (ImGui::Checkbox("Draw Wireframe", &wireframe))
            renderer.SetWireframe(wireframe);

        bool depthTest = renderer.DepthTest();
        if (ImGui::Checkbox("Draw Depth", &depthTest))
            renderer.SetDepthTest(depthTest);

        ImGui::SliderFloat("Ambient Intensity", &renderer.AmbientIntensity(), 0.2f, 0.5f);

        ImGui::PopID();
    }

    // Directional Light
    if (ImGui::CollapsingHeader("Direction Light", ImGuiTreeNodeFlags_None))
    {
        ImGui::PushID("DirLight");

        auto &light = scene.GetDirectionalLight();
        ImGui::Checkbox("Enabled", &light.enabled);
        ImGui::ColorPicker3("Light Color", glm::value_ptr(light.color));
        ImGui::DragFloat3("Direction", glm::value_ptr(light.direction));

        ImGui::PopID();
    }

    // Point Light
    if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_None))
    {
        ImGui::PushID("PointLight");

        auto &light = scene.GetPointLight();
        ImGui::Checkbox("Enabled", &light.enabled);
        ImGui::ColorPicker3("Light Color", glm::value_ptr(light.color));
        ImGui::DragFloat3("Position", glm::value_ptr(light.position));
        ImGui::DragFloat("Constant", &light.constant, 0.01f, 0.1f, 2.0f);
        ImGui::DragFloat("Linear", &light.linear, 0.001f, 0.0f, 0.5f);
        ImGui::DragFloat("Quadratic", &light.quadratic, 0.001f, 0.0f, 0.3f);

        // --- Attenuation curve preview ---
        constexpr int kSampleCount = 100;
        constexpr float kMaxDistance = 50.0f;
        static float samples[kSampleCount];

        for (int i = 0; i < kSampleCount; ++i)
        {
            float d = (float)i / (kSampleCount - 1) * kMaxDistance;
            float atten = 1.0f / (light.constant + light.linear * d + light.quadratic * d * d);
            samples[i] = atten;
        }

        ImGui::PlotLines("Attenuation", samples, kSampleCount, 0, nullptr, 0.0f, 1.0f, ImVec2(0, 100));
        ImGui::Text(
            "Range (attn < 0.05): %.1f units",
            [&]
            {
                for (int i = 0; i < kSampleCount; ++i)
                    if (samples[i] < 0.05f)
                        return (float)i / (kSampleCount - 1) * kMaxDistance;
                return kMaxDistance;
            }());

        ImGui::PopID();
    }

    // Spot Light
    if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_None))
    {
        ImGui::PushID("SpotLight");

        auto &light = scene.GetSpotLight();
        ImGui::Checkbox("Enabled", &light.enabled);
        ImGui::ColorPicker3("Light Color", glm::value_ptr(light.color));
        ImGui::DragFloat("Inner Cutoff", &light.cutOff, 1.0f, 1.0f, 20.0f);
        ImGui::DragFloat("Outer Cutoff", &light.outerCutOff, 1.0f, 5.0f, 20.0f);

        ImGui::PopID();
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Render()
{
    renderer.Render(scene, window);

    if (state == AppState::DebugMode)
        DrawDebugUI();

    window.SwapBuffers();
};

void Engine::Update(float dt)
{
    std::shared_ptr<Camera> camera = scene.GetSceneCamera();

    // Camera Keyboard Movement
    if (state == AppState::GameMode)
    {
        if (input.IsKeyDown(SDL_SCANCODE_W))
            camera->ProcessKeyboardMovement(dt, Direction::Forward);
        if (input.IsKeyDown(SDL_SCANCODE_S))
            camera->ProcessKeyboardMovement(dt, Direction::BackWard);
        if (input.IsKeyDown(SDL_SCANCODE_A))
            camera->ProcessKeyboardMovement(dt, Direction::Left);
        if (input.IsKeyDown(SDL_SCANCODE_D))
            camera->ProcessKeyboardMovement(dt, Direction::Right);
        if (input.IsKeyDown(SDL_SCANCODE_SPACE))
            camera->ProcessKeyboardMovement(dt, Direction::Up);
        if (input.IsKeyDown(SDL_SCANCODE_LSHIFT))
            camera->ProcessKeyboardMovement(dt, Direction::Down);

        camera->ProcessMouseMovement(input.MouseDeltaX(), input.MouseDeltaY());
    }
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
            case SDL_EVENT_MOUSE_WHEEL:
                if (state == AppState::GameMode)
                    scene.GetSceneCamera()->ProcessZoom(e.wheel.y);
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
