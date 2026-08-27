#include "Scene.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <memory>

const char *earthModel = "res/models/earth/earth.obj";
const char *backpackModel = "res/models/backpack/backpack.obj";
const char *moonModel = "res/models/moon/moon.obj";
const int count = 2;

void Scene::Load()
{
    m_pointLight.gizmo = std::make_unique<Model>(moonModel);

    auto backpack = std::make_shared<Model>(backpackModel); // load ONCE

    // Multiple obj render test
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            SceneObject obj{backpack};
            obj.position = glm::vec3(i * 5.0f, 0.0f, j * -5.0f);
            objects.push_back(std::move(obj));
        }
    }
}
