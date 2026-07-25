#pragma once

#include <vector>
// #include <memory>
#include <glm/glm.hpp>
#include "renderer/Model.hpp"

// A single drawable instance: a model plus where it sits in the world.
struct SceneObject
{
    Model model;
    glm::mat4 transform{1.0f};
};

class Scene
{
  public:
    void Load();

    std::vector<SceneObject> &Objects() { return objects; }
    const std::vector<SceneObject> &Objects() const { return objects; }

    glm::vec3 &LightColor() { return lightColor; }
    const glm::vec3 &LightColor() const { return lightColor; }

    glm::vec4 &DirectionalLight() { return directionalLight; }
    const glm::vec4 &DirectionalLight() const { return directionalLight; }

  private:
    std::vector<SceneObject> objects;
    std::unique_ptr<Model> lightMarker;

    glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
    glm::vec4 directionalLight{-0.2f, -1.0f, -0.3f, 0.0f};
};
