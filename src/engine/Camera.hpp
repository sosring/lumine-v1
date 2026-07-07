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

    void DebugUI()
    {
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Position", &Position.x, 0.05f);
            ImGui::Text("Front: (%.2f, %.2f, %.2f)", Front.x, Front.y, Front.z);
            ImGui::DragFloat("Yaw", &Yaw, 0.5f);
            ImGui::DragFloat("Pitch", &Pitch, 0.5f, -89.0f, 89.0f);
            // ImGui::SliderFloat("FOV", &fov, 1.0f, 120.0f);
        }
    }

    // Call once per frame with SDL's relative mouse delta (e.g. from SDL_EVENT_MOUSE_MOTION's xrel/yrel)
    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        Yaw += xoffset * sensitivity;
        Pitch -= yoffset * sensitivity; // inverted: moving mouse up should look up

        // Prevent gimbal-flip at the poles
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        updateVectors();
    }

    void ProcessKeyboardMovement(float dt, Direction dir)
    {
        glm::vec3 movement(0.0f);

        switch (dir)
        {
            case Direction::Forward:
                movement += Front;
                break;
            case Direction::BackWard:
                movement -= Front;
                break;
            case Direction::Left:
                movement -= Right;
                break;
            case Direction::Right:
                movement += Right;
                break;
            case Direction::Up:
                movement += WorldUp;
                break;
            case Direction::Down:
                movement -= WorldUp;
                break;
        }

        if (glm::length(movement) > 0.0f)
        {
            // Flatten movement to the XZ plane so W/S don't fly you up/down when pitched.
            // Remove these two lines if you want free-fly (noclip-style) movement instead.
            movement.y = (dir == Direction::Up || dir == Direction::Down) ? movement.y : 0.0f;

            movement = glm::normalize(movement);
            Position += movement * Speed * dt;
        }
    }

  private:
    void updateVectors()
    {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        newFront.y = sin(glm::radians(Pitch));
        newFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(newFront);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

    glm::vec3 Position{0.0f, 1.5f, 3.0f};
    glm::vec3 Front{0.0f, 0.0f, -1.0f};
    glm::vec3 Up{0.0f, 1.0f, 0.0f};
    glm::vec3 Right{1.0f, 0.0f, 0.0f};
    glm::vec3 WorldUp{0.0f, 1.0f, 0.0f};

    float Yaw{-90.0f}; // -90 so default Front points down -Z, matching old default
    float Pitch{-25.0f};

    const float Speed{2.5f};
    const float sensitivity{0.1f};
};
