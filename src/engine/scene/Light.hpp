#include <glm/glm.hpp>
#include "renderer/Model.hpp"

struct Light
{
    bool enabled{true};
    glm::vec3 color{1.0f};
};

struct DirectionalLight : Light
{
    glm::vec3 direction{3.0f, 3.0f, -3.0f};
    glm::vec3 color{0.2f, 0.2f, 0.7f};
};

struct PointLight : Light
{
    bool enabled{false};
    glm::vec3 position{3.0f, 3.0f, -3.0f};
    glm::vec3 color{1.0f, 0.5f, 0.4f};
    float constant = 1.0f, linear = 0.07f, quadratic = 0.017f;
    std::unique_ptr<Model> gizmo; // the sphere mesh drawn to visualize the light
};

struct SpotLight : Light
{
    // Position and direction will be set from camera
    glm::vec3 color{1.0f};
    float cutOff{5.0f};
    float outerCutOff{10.0f};
};
