#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

enum class Direction
{
    Forward,
    BackWard,
    Left,
    Right,
    Up,
    Down
};

class Camera
{
  public:
    Camera() { updateVectors(); }

    glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

    glm::vec3 GetPosition() const { return Position; }

    void DebugUI();

    // Call once per frame with SDL's relative mouse delta (e.g. from SDL_EVENT_MOUSE_MOTION's xrel/yrel)
    void ProcessMouseMovement(float xoffset, float yoffset);

    void ProcessKeyboardMovement(float dt, Direction dir);

  private:
    void updateVectors();

    glm::vec3 Position{0.0f, 6.0f, 5.0f};
    glm::vec3 Front{0.0f, 0.0f, -1.0f};
    glm::vec3 Up{0.0f, 1.0f, 0.0f};
    glm::vec3 Right{1.0f, 0.0f, 0.0f};
    glm::vec3 WorldUp{0.0f, 1.0f, 0.0f};

    float Yaw{-90.0f}; // -90 so default Front points down -Z, matching old default
    float Pitch{-45.0f};

    const float Speed{3.0f};
    const float sensitivity{0.1f};
};
