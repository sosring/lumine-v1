#include "Renderer.hpp"
#include "scene/Scene.hpp"
#include <memory>

Renderer::Renderer()
{
    modelShader = std::make_unique<Shader>("res/shaders/model.vs", "res/shaders/point_light.fs");
    lightShader = std::make_unique<Shader>("res/shaders/moon.vs", "res/shaders/moon.fs");
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
    modelShader->setInt("wireframe", wireframe);

    glm::vec3 lightPosView = glm::vec3(view * glm::vec4(scene.GetPointLight().position, 1.0f));
    glm::vec3 lightDirView = glm::vec3(view * glm::vec4(scene.GetDirectionalLight().direction, 1.0f));
    modelShader->setVec3("light.direction", lightDirView);
    modelShader->setVec3("light.position", lightPosView);
    modelShader->setFloat("ambientIntensity", ambientIntensity);
    modelShader->setVec3("light.color", scene.GetDirectionalLight().color);

    modelShader->setFloat("light.constant", 1.0f);
    modelShader->setFloat("light.linear", 0.09f);
    modelShader->setFloat("light.quadratic", 0.032f);

    for (auto &object : scene.Objects())
    {
        modelShader->setMat4("model", object.transform);
        object.model.Draw(*modelShader);
    }

    // Light Source
    lightShader->Use();
    lightShader->setMat4("view", view);
    lightShader->setMat4("projection", projection);
    lightShader->setVec3("lightColor", scene.GetPointLight().color);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), scene.GetPointLight().position);
    lightShader->setMat4("model", model);

    scene.GetPointLight().gizmo->Draw(*lightShader);
}
