#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "renderer/Model.hpp"

// A single drawable instance: a model plus where it sits in the world.
struct SceneObject
{
    Model model;
    glm::mat4 transform{1.0f};
};

struct Light
{
    glm::vec3 color{1.0f};
    bool active;
};

struct DirectionalLight : Light
{
    glm::vec3 direction{3.0f, 3.0f, -3.0f};
    glm::vec3 color{0.2f, 0.2f, 0.7f};
};

struct PointLight : Light
{
    glm::vec3 position{3.0f, 3.0f, -3.0f};
    glm::vec3 color{1.0f, 0.5f, 0.4f};
    float constant = 1.0f, linear = 0.07f, quadratic = 0.017f;
    std::unique_ptr<Model> gizmo; // the sphere mesh drawn to visualize the light
};

struct SpotLight : Light
{
    // Position and direction will be set from camera
    // glm::vec3 direction{1.0f};
    // glm::vec3 position{1.0f};
    glm::vec3 color{1.0f};
    float cutOff{5.0f};
    float outerCutOff{10.0f};
};

class Scene
{
  public:
    void Load();

    std::vector<SceneObject> &Objects() { return objects; }
    const std::vector<SceneObject> &Objects() const { return objects; }

    DirectionalLight &GetDirectionalLight() { return m_DirectionalLight; }
    const DirectionalLight &GetDirectionalLight() const { return m_DirectionalLight; }

    PointLight &GetPointLight() { return m_pointLight; }
    const PointLight &GetPointLight() const { return m_pointLight; }

    SpotLight &GetSpotLight() { return m_spotLight; }
    const SpotLight &GetSpotLight() const { return m_spotLight; }

  private:
    std::vector<SceneObject> objects;

    DirectionalLight m_DirectionalLight;
    PointLight m_pointLight;
    SpotLight m_spotLight;
};
