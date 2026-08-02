#include "Renderer.hpp"
#include "scene/Scene.hpp"
#include "scene/Camera.hpp"

Renderer::Renderer()
{
    modelShader = std::make_unique<Shader>("res/shaders/model.vs", "res/shaders/multiple_light.fs");
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

    auto &dirLight = scene.GetDirectionalLight();
    auto &pointLight = scene.GetPointLight();
    auto &spotLight = scene.GetSpotLight();

    // Directional Lighting
    {
        glm::vec3 lightDirView = glm::vec3(view * glm::vec4(dirLight.direction, 0.0f));

        modelShader->setVec3("dirLight.direction", lightDirView);
        modelShader->setVec3("dirLight.color", dirLight.color);
        modelShader->setFloat("ambientIntensity", ambientIntensity);
    }

    // Point Light
    {
        glm::vec3 lightPosView = glm::vec3(view * glm::vec4(pointLight.position, 1.0f));

        modelShader->setVec3("pointLights[0].position", lightPosView);
        modelShader->setVec3("pointLights[0].color", pointLight.color);
        modelShader->setFloat("pointLights[0].constant", pointLight.constant);
        modelShader->setFloat("pointLights[0].linear", pointLight.linear);
        modelShader->setFloat("pointLights[0].quadratic", pointLight.quadratic);
    }

    // Spot Light
    {
        glm::vec3 lightPosView = glm::vec3(view * glm::vec4(camera.GetPosition(), 1.0f));
        glm::vec3 lightDirView = glm::vec3(view * glm::vec4(camera.GetFront(), 0.0f));

        modelShader->setVec3("spotLight.position", lightPosView);
        modelShader->setVec3("spotLight.direction", lightDirView);
        modelShader->setVec3("spotLight.color", spotLight.color);
        modelShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(spotLight.cutOff)));
        modelShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLight.outerCutOff)));
    }

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
