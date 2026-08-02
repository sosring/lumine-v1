#pragma once

#include <glad.h>
#include <SDL3/SDL.h>
#include <stdexcept>

class Window
{
  public:
    Window(const char *title, int width, int height) : width(width), height(height)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_Log("Failed to initialize SDL");
            throw std::runtime_error("Failed to initialize SDL");
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        handle = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!handle)
        {
            SDL_Log("Failed to create window: %s", SDL_GetError());
            throw std::runtime_error("window creation failed");
        }

        glContext = SDL_GL_CreateContext(handle);
        if (!glContext)
        {
            SDL_Log("Failed to create GL context: %s", SDL_GetError());
            throw std::runtime_error("Failed to create gl context");
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            SDL_Log("Failed to load GL proc addresses: %s", SDL_GetError());
            throw std::runtime_error("Failed to load proc address");
        }

        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
    }

    ~Window()
    {
        if (glContext)
            SDL_GL_DestroyContext(glContext);
        if (handle)
            SDL_DestroyWindow(handle);
        SDL_Quit();
    }

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void SwapBuffers() { SDL_GL_SwapWindow(handle); }

    void Resize(int w, int h)
    {
        width = w;
        height = h;
        glViewport(0, 0, width, height);
    }

    void SetRelativeMouseMode(bool enabled) { SDL_SetWindowRelativeMouseMode(handle, enabled); }

    SDL_Window *Handle() const { return handle; }
    SDL_GLContext GLContext() const { return glContext; }

    int Width() const { return width; }
    int Height() const { return height; }
    float AspectRatio() const { return height != 0 ? (float)width / (float)height : 1.0f; }

  private:
    SDL_Window *handle = nullptr;
    SDL_GLContext glContext = nullptr;
    int width, height;
};
