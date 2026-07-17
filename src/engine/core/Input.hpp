#pragma once

#include <SDL3/SDL.h>

class Input
{
  public:
    // Call once per frame BEFORE processing events (clears per-frame deltas)
    void BeginFrame()
    {
        mouseDeltaX = 0.0f;
        mouseDeltaY = 0.0f;
    }

    // Feed raw SDL events in; Engine still owns the poll loop (so it can
    // also forward events to ImGui), this just updates Input's own state.
    void ProcessEvent(const SDL_Event &e)
    {
        switch (e.type)
        {
            case SDL_EVENT_QUIT:
                quitRequested = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (e.key.key == SDLK_ESCAPE)
                    quitRequested = true;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                mouseDeltaX += e.motion.xrel;
                mouseDeltaY += e.motion.yrel;
                break;
            default:
                break;
        }
    }

    bool IsKeyDown(SDL_Scancode key) const { return SDL_GetKeyboardState(nullptr)[key]; }

    bool QuitRequested() const { return quitRequested; }
    void ClearQuit() { quitRequested = false; } // in case Engine wants to override/reset it

    float MouseDeltaX() const { return mouseDeltaX; }
    float MouseDeltaY() const { return mouseDeltaY; }

  private:
    bool quitRequested = false;
    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;
};
