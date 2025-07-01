#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

constexpr int MAX_TEXTURE_UNITS = 16;

class Object3D {
public:
    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    glm::vec3 scale = {1, 1, 1};

    std::shared_ptr<Mesh> mesh = {};
    std::vector<Texture*> textures;
    Shader* shader = {};

    explicit Object3D(Mesh* mesh) : mesh(mesh) {}

    [[nodiscard]] glm::mat4 getModelMatrix() const {
        glm::mat4 mat = translate(glm::mat4(1.0), {position.x, position.y, position.z});
        mat = rotate(mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        mat = rotate(mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        mat = rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        mat = glm::scale(mat, {scale.x, scale.y, scale.z});
        return mat;
    }
};

#endif //OBJECT3D_H
