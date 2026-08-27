#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Light.hpp"
#include "SceneObject.hpp"
#include "Camera.hpp"

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

    const std::shared_ptr<Camera> &GetSceneCamera() const { return m_sceneCamera; }

  private:
    std::vector<SceneObject> objects;
    std::shared_ptr<Camera> m_sceneCamera = std::make_shared<Camera>();

    DirectionalLight m_DirectionalLight;
    PointLight m_pointLight;
    SpotLight m_spotLight;
};
