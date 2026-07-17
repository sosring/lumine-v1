#include "Camera.hpp"

// Call once per frame with SDL's relative mouse delta (e.g. from SDL_EVENT_MOUSE_MOTION's xrel/yrel)
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
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

void Camera::ProcessKeyboardMovement(float dt, Direction dir)
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

void Camera::updateVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    newFront.y = sin(glm::radians(Pitch));
    newFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(newFront);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
