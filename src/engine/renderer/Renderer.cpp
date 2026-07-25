#include "Renderer.hpp"
#include "scene/Scene.hpp"

Renderer::Renderer()
{
    modelShader = std::make_unique<Shader>("res/shaders/model.vs", "res/shaders/model.fs");
}

void Renderer::Render(const Scene &scene, Camera &camera, const Window &window)
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), window.AspectRatio(), 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    // Scene objects
    modelShader->Use();
    modelShader->setMat4("view", view);
    modelShader->setMat4("projection", projection);
    modelShader->setVec3("light.color", scene.LightColor());
    modelShader->setFloat("ambientIntensity", ambientIntensity);
    modelShader->setVec4("light.direction", scene.DirectionalLight());
    modelShader->setInt("wireframe", wireframe);

    for (auto &object : scene.Objects())
    {
        modelShader->setMat4("model", object.transform);
        object.model.Draw(*modelShader);
    }
}
