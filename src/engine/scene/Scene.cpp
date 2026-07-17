#include "Scene.hpp"

void Scene::Load()
{
    objects.push_back(SceneObject{Model("res/models/teapot/teapot.obj")});
    // objects.push_back(SceneObject{Model("res/models/backpack/backpack.obj")});

    lightMarker = std::make_unique<Model>("res/models/moon/moon.obj");
}
