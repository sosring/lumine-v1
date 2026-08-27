#include "Renderer.hpp"
#include "scene/Scene.hpp"

Renderer::Renderer()
{
    m_modelShader = std::make_unique<Shader>("res/shaders/model.vs", "res/shaders/multiple_light.fs");
    m_lightShader = std::make_unique<Shader>("res/shaders/moon.vs", "res/shaders/moon.fs");
    m_outlineShader = std::make_unique<Shader>("res/shaders/model.vs", "res/shaders/outline.fs");
}

void Renderer::Render(const Scene &scene, const Window &window)
{
    const auto &camera = scene.GetSceneCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 projection = camera->GetProjectionMatrix(window);
    glm::mat4 view = camera->GetViewMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);

    // Scene objects
    m_modelShader->Use();
    m_modelShader->setMat4("view", view);
    m_modelShader->setMat4("projection", projection);
    m_modelShader->setInt("wireframe", m_wireframe);
    m_modelShader->setInt("depthTest", m_depthTest);

    auto &dirLight = scene.GetDirectionalLight();
    auto &pointLight = scene.GetPointLight();
    auto &spotLight = scene.GetSpotLight();

    // Directional Lighting
    {
        glm::vec3 lightDirView = glm::vec3(view * glm::vec4(dirLight.direction, 0.0f));

        m_modelShader->setBool("dirLight.enabled", dirLight.enabled);
        m_modelShader->setVec3("dirLight.direction", lightDirView);
        m_modelShader->setVec3("dirLight.color", dirLight.color);
        m_modelShader->setFloat("ambientIntensity", m_ambientIntensity);
    }

    // Point Light
    {
        glm::vec3 lightPosView = glm::vec3(view * glm::vec4(pointLight.position, 1.0f));

        m_modelShader->setBool("pointLights[0].enabled", pointLight.enabled);
        m_modelShader->setVec3("pointLights[0].position", lightPosView);
        m_modelShader->setVec3("pointLights[0].color", pointLight.color);
        m_modelShader->setFloat("pointLights[0].constant", pointLight.constant);
        m_modelShader->setFloat("pointLights[0].linear", pointLight.linear);
        m_modelShader->setFloat("pointLights[0].quadratic", pointLight.quadratic);
    }

    // Spot Light
    {
        glm::vec3 lightPosView = glm::vec3(view * glm::vec4(camera->GetPosition(), 1.0f));
        glm::vec3 lightDirView = glm::vec3(view * glm::vec4(camera->GetFront(), 0.0f));

        m_modelShader->setBool("spotLight.enabled", spotLight.enabled);
        m_modelShader->setVec3("spotLight.position", lightPosView);
        m_modelShader->setVec3("spotLight.direction", lightDirView);
        m_modelShader->setVec3("spotLight.color", spotLight.color);
        m_modelShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(spotLight.cutOff)));
        m_modelShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLight.outerCutOff)));
    }

    // Render Light Source
    {
        m_lightShader->Use();
        m_lightShader->setMat4("view", view);
        m_lightShader->setMat4("projection", projection);
        m_lightShader->setVec3("lightColor", scene.GetPointLight().color);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), scene.GetPointLight().position);
        m_lightShader->setMat4("model", model);

        if (pointLight.enabled)
            scene.GetPointLight().gizmo->Draw(*m_lightShader);
    }

    {
        // Render Model
        m_outlineShader->Use();
        m_outlineShader->setMat4("view", view);
        m_outlineShader->setMat4("projection", projection);

        for (auto &object : scene.Objects())
        {
            object.model->DrawWithOutline(*m_modelShader, *m_outlineShader, object.GetModelMatrix());
        }

        glDisable(GL_STENCIL_TEST);
    }
}
