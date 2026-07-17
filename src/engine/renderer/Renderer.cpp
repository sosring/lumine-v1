#include "Renderer.hpp"

Renderer::Renderer()
{
    objectShader = std::make_unique<Shader>("res/shaders/object.vs", "res/shaders/object.fs");
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
    objectShader->Use();
    objectShader->setMat4("view", view);
    objectShader->setMat4("projection", projection);
    objectShader->setVec3("lightColor", scene.LightColor());
    objectShader->setVec3("lightPos", scene.LightPosition());

    for (auto &object : scene.Objects())
    {
        objectShader->setMat4("model", object.transform);
        object.model.Draw(*objectShader);
    }

    // Light marker
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), scene.LightPosition());
    lightModel = glm::scale(lightModel, glm::vec3(0.5f));

    lightShader->Use();
    lightShader->setMat4("model", lightModel);
    lightShader->setMat4("view", view);
    lightShader->setMat4("projection", projection);
    lightShader->setVec3("lightColor", scene.LightColor());

    scene.LightMarker().Draw(*lightShader);
}
