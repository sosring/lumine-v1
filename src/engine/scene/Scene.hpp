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

struct DirectionalLight
{
    glm::vec3 direction{3.0f, -1.0f, -0.3f};
    glm::vec3 color{1.0f, 1.0f, 1.0f};
};

struct PointLight
{
    glm::vec3 position{3.0f, 3.0f, -3.0f};
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    float constant = 1.0f, linear = 0.07f, quadratic = 0.017f;
    std::unique_ptr<Model> gizmo; // the sphere mesh drawn to visualize the light
};

struct SpotLight
{
    // Position and direction will be set from camera
    // glm::vec3 direction{1.0f};
    // glm::vec3 position{1.0f};
    glm::vec3 color{1.0f, 1.0f, 1.0f};
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

    PointLight &GetPointLight() { return m_PointLight; }
    const PointLight &GetPointLight() const { return m_PointLight; }

    SpotLight &GetSpotLight() { return m_SpotLight; }
    const SpotLight &GetSpotLight() const { return m_SpotLight; }

  private:
    std::vector<SceneObject> objects;

    DirectionalLight m_DirectionalLight;
    PointLight m_PointLight;
    SpotLight m_SpotLight;
};
