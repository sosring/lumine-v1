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

    void SetWireframe(bool enabled) { wireframe = enabled; }
    bool Wireframe() const { return wireframe; }

    float &AmbientIntensity() { return ambientIntensity; }
    const float &AmbientIntensity() const { return ambientIntensity; }

  private:
    std::unique_ptr<Shader> modelShader;
    std::unique_ptr<Shader> lightShader;
    bool wireframe = false;
    float ambientIntensity = 0.2f;
};
