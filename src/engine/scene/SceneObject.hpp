#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp> // needed for glm::quat, glm::mat4_cast
#include <memory>

#include "renderer/Model.hpp"

// A single drawable instance: a model plus where it sits in the world.
struct SceneObject
{
    std::shared_ptr<Model> model;               // shared, not owned uniquely
    glm::vec3 position{1.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f}; // identity quat
    glm::vec3 scale{1.0f};

    glm::mat4 GetModelMatrix() const
    {
        glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 r = glm::mat4_cast(rotation);
        glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
        return t * r * s;
    }
};
