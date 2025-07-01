#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <vector>

#include "Shader.h"

class ShaderManager {
    std::pmr::vector<Shader*> shaders;

public:
    void registerShader(Shader* shader) {
        shaders.push_back(shader);
    }

    void injectGlobals(const glm::mat4& view, const glm::mat4& proj) const {
        for (int i = 0; i < shaders.size(); ++i) {
            shaders[i]->use();
            shaders[i]->setMatrix4("view", view);
            shaders[i]->setMatrix4("projection", proj);
        }
    }

    void useShaders() const {
        for (int i = 0; i < shaders.size(); ++i) {
            shaders[i]->use();
        }
    }
};

#endif //SHADERMANAGER_H
