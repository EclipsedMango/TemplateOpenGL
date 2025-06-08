#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <glm/glm.hpp>

#include "mesh.h"
#include "texture.h"
#include <glm/gtc/matrix_transform.hpp>

constexpr int MAX_TEXTURE_UNITS = 16;

class Object3D {
public:
    glm::vec3 position = glm::vec3(0);
    glm::vec3 rotation = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1.0f);

    Mesh* mesh = {};
    std::vector<Texture*> textures;

    glm::mat4 getModelMatrix() const {
        glm::mat4 mat = translate(glm::mat4(1.0f), position);
        mat = rotate(mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        mat = rotate(mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        mat = rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        mat = glm::scale(mat, scale);
        return mat;
    }

    void bindMeshBuffers() const {
        mesh->bindBuffers();
    }

    void deleteBuffers() const {
        mesh->deleteBuffers();
    }

    void draw(const Shader& shader) const {
        shader.use();
        shader.setMatrix4("model", getModelMatrix());

        if (!textures.empty()) {
            for (int i = 0; i < textures.size(); ++i) {
                if (i < MAX_TEXTURE_UNITS) {
                    textures[i]->bind(GL_TEXTURE0 + i);
                    shader.setInt("texture" + std::to_string(i + 1), i);
                }
            }
        }

        if (mesh) {
            mesh->draw();
        }
    }
};

#endif //OBJECT3D_H
