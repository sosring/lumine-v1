#include "Scene.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <memory>

const char *earthModel = "res/models/earth/earth.obj";
const char *backpackModel = "res/models/backpack/backpack.obj";
const char *moonModel = "res/models/moon/moon.obj";
const int count = 10;

void Scene::Load()
{
    // objects.push_back(SceneObject{Model(backpackModel)});
    m_pointLight.gizmo = std::make_unique<Model>(moonModel);

    // Multiple obj render test
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            SceneObject obj{Model(earthModel)};
            obj.transform = glm::translate(obj.transform, glm::vec3(i * 3.0f, 0.0f, j * -3.0f));
            obj.transform = glm::rotate(obj.transform, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            objects.push_back(std::move(obj));
        }
    }
}
