#include "Engine.hpp"
#include "engine/Camera.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

/* clang-format off */
const float vertices[] = {
    // Position              // Color             // UV

    // Front (+Z)
    -0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

    // Back (-Z)
     0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,    0.0f, 1.0f,

    // Left (-X)
    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    // Right (+X)
     0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 0.0f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 0.0f,    0.0f, 1.0f,

    // Bottom (-Y)
    -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 1.0f,    1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    // Top (+Y)
    -0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 1.0f,    0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 1.0f,    1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 1.0f,    0.0f, 1.0f,
};

const GLuint indices[] = {
    0, 1, 2, 2, 3, 0,         // Front
    4, 5, 6, 6, 7, 4,         // Back
    8, 9,10,10,11, 8,         // Left
   12,13,14,14,15,12,         // Right
   16,17,18,18,19,16,         // Bottom
   20,21,22,22,23,20          // Top
};
/* clang-format on */

Engine::Engine(int width, int height) : width(width), height(height)
{
    InitWindow();
    InitGL();
    InitScene();
    InitImGui();

    lastTicks = SDL_GetTicks();
};

Engine::~Engine()
{
    Shutdown();
}

void Engine::InitWindow()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL");
        throw std::runtime_error("Failed to initialize SDL");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Lumine", width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Failed to create window %s", SDL_GetError());
        throw std::runtime_error("window creation failed");
    }

    if (state == AppState::GameMode)
        SDL_SetWindowRelativeMouseMode(window, true);
};

void Engine::InitGL()
{
    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        SDL_Log("Failed to create gl context %s", SDL_GetError());
        throw std::runtime_error("Failed to create gl context");
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("Failed to load proc address %s", SDL_GetError());
        throw std::runtime_error("Failed to load proc address");
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
};

void Engine::InitScene()
{
    shader = std::make_unique<Shader>("res/shaders/box.vs", "res/shaders/box.fs");

    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    ebo = std::make_unique<IndexBuffer>(indices, sizeof(indices));

    vao->LinkVertexBuffer(*vbo, 0, 3, GL_FLOAT, sizeof(GLfloat) * 8, (void *)0);
    vao->LinkVertexBuffer(*vbo, 1, 3, GL_FLOAT, sizeof(GLfloat) * 8, (void *)(3 * sizeof(float)));
    vao->LinkVertexBuffer(*vbo, 2, 2, GL_FLOAT, sizeof(GLfloat) * 8, (void *)(6 * sizeof(float)));

    shader->Use();
    textureBrick = std::make_unique<Texture>("res/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    shader->setInt("tex0", 0);

    textureCat = std::make_unique<Texture>("res/textures/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
    shader->setInt("tex1", 1);
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
    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 410");
};

void Engine::DrawScene()
{
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    shader->Use();
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    vao->Bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);
};

void Engine::DrawDebugUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ImGui::ShowDemoWindow();

    ImGui::Begin("Debug");
    camera.DebugUI();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Render()
{
    glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawScene();
    if (state == AppState::DebugMode)
        DrawDebugUI();

    SDL_GL_SwapWindow(window);
};

void Engine::Update(float dt)
{
    // Update camera and scene

    // Camera Keyboard Movement
    const bool *keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_W])
        camera.ProcessKeyboardMovement(dt, Direction::Forward);
    if (keys[SDL_SCANCODE_S])
        camera.ProcessKeyboardMovement(dt, Direction::BackWard);
    if (keys[SDL_SCANCODE_A])
        camera.ProcessKeyboardMovement(dt, Direction::Left);
    if (keys[SDL_SCANCODE_D])
        camera.ProcessKeyboardMovement(dt, Direction::Right);
    if (keys[SDL_SCANCODE_SPACE])
        camera.ProcessKeyboardMovement(dt, Direction::Up);
    if (keys[SDL_SCANCODE_LSHIFT])
        camera.ProcessKeyboardMovement(dt, Direction::Down);
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

void Engine::Shutdown()
{
    textureCat.reset();
    textureBrick.reset();
    ebo.reset();
    vbo.reset();
    vao.reset();
    shader.reset();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    if (glContext)
    {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
};

void Engine::PollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSDL3_ProcessEvent(&e);

        switch (e.type)
        {
            case SDL_EVENT_QUIT:
                quit = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (e.key.key == SDLK_ESCAPE)
                    quit = true;
                if (e.key.key == SDLK_Q)
                {
                    state = state == AppState::DebugMode ? AppState::GameMode : AppState::DebugMode;
                    SDL_SetWindowRelativeMouseMode(window, state == AppState::GameMode);
                }
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                width = e.window.data1;
                height = e.window.data2;
                glViewport(0, 0, width, height);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                if (state == AppState::GameMode)
                    camera.ProcessMouseMovement(e.motion.xrel, e.motion.yrel);
                break;
        }
    }
};
