#pragma once

#include <memory>
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
    glm::vec3 position{0.0f, 3.0f, -3.0f};
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    float constant = 1.0f, linear = 0.09f, quadratic = 0.032f;
    std::unique_ptr<Model> gizmo; // the sphere mesh drawn to visualize the light
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

  private:
    std::vector<SceneObject> objects;

    DirectionalLight m_DirectionalLight;
    PointLight m_PointLight;
};
