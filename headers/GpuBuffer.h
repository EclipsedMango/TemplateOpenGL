#ifndef GPUBUFFER_H
#define GPUBUFFER_H

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
};

class GpuBuffer {
public:
    virtual ~GpuBuffer() = default;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    int indexCount = 0;
};

class OpenGLGpuBuffer : public GpuBuffer {
public:
    OpenGLGpuBuffer(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        this->indexCount = static_cast<int>(indices.size());
        _setupBuffers(vertices, indices);
    }
    ~OpenGLGpuBuffer() override {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    void _setupBuffers(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position)));

        // Normals will go here, and will follow the same offset pattern, it will be part of vertex.

        // TexCoord
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoord)));

        glBindVertexArray(0);
    }
};

class VulkanGpuBuffer : public GpuBuffer {};

#endif //GPUBUFFER_H
