#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "Shader.hpp"
#include "scene/Scene.hpp"
#include "scene/Camera.hpp"
#include "core/Window.hpp"

class Renderer
{
  public:
    Renderer();

    void Render(const Scene &scene, Camera &camera, const Window &window);

    void SetWireframe(bool enabled) { m_wireframe = enabled; }
    bool Wireframe() const { return m_wireframe; }

    bool DepthTest() const { return m_depthTest; }
    void SetDepthTest(bool enabled) { m_depthTest = enabled; }

    float &AmbientIntensity() { return m_ambientIntensity; }
    const float &AmbientIntensity() const { return m_ambientIntensity; }

  private:
    std::unique_ptr<Shader> m_modelShader;
    std::unique_ptr<Shader> m_lightShader;
    bool m_wireframe = false;
    bool m_depthTest = false;
    float m_ambientIntensity = 0.2f;
};
