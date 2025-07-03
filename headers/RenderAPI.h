#ifndef RENDERAPI_H

#define RENDERAPI_H
#include <memory>

#include "Object3d.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "MathHeaders/Colour.h"

class RenderAPI {
public:
    RenderAPI() = default;
    virtual ~RenderAPI() = default;

    virtual void init() = 0;
    virtual void registerObject(const Object3D* object) = 0;

    virtual std::unique_ptr<GpuBuffer> CreateGpuBuffer(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) = 0;

    virtual void startDrawing() = 0;
    virtual void endDrawing(Window* window) = 0;

    virtual void drawObject(const Object3D &object) = 0;
    virtual void drawRegisteredObjects() = 0;

    virtual void setClearColour(emc::Colour colour) = 0;
    virtual void setClearColour(float r, float g, float b, float a) = 0;

    virtual float getFrameTime() = 0;

protected:
    float m_DeltaTime = 0.0f;
    float m_LastFrame = 0.0f;
};

class OpenGlRenderAPI : public RenderAPI {
public:
    OpenGlRenderAPI() { m_LastFrame = static_cast<float>(glfwGetTime()); }
    ~OpenGlRenderAPI() override = default;

    void init() override {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Failed to initialize GLAD");
        }
        glEnable(GL_DEPTH_TEST);
    }

    void startDrawing() override { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    void endDrawing(Window* window) override {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        m_DeltaTime = currentFrameTime - m_LastFrame;
        m_LastFrame = currentFrameTime;

        window->swapBuffers();
    }

    std::unique_ptr<GpuBuffer> CreateGpuBuffer(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) override {
        return std::make_unique<OpenGLGpuBuffer>(vertices, indices);
    }

    void registerObject(const Object3D* object) override { m_RegisteredObjects.push_back(object); }
    void drawRegisteredObjects() override {
        for (int i = 0; i < m_RegisteredObjects.size(); ++i) {
            if (m_RegisteredObjects[i]) {
                drawObject(*m_RegisteredObjects[i]);
            }
        }
    }

    void drawObject(const Object3D &object) override {
        if (!object.mesh || !object.mesh->gpuBuffer || !object.shader) { return; }

        object.shader->use();

        if (!object.textures.empty()) {
            for (int i = 0; i < object.textures.size(); ++i) {
                if (i < MAX_TEXTURE_UNITS) {
                    object.textures[i]->bind(GL_TEXTURE0 + i);
                    object.shader->setInt("texture" + std::to_string(i + 1), i);
                }
            }
        }

        object.shader->setMatrix4("model", object.getModelMatrix());

        const OpenGLGpuBuffer* buffer = dynamic_cast<const OpenGLGpuBuffer*>(object.mesh->gpuBuffer.get());

        glBindVertexArray(buffer->VAO);
        glDrawElements(GL_TRIANGLES, buffer->indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void setClearColour(const emc::Colour colour) override { setClearColour(colour.GetRed(), colour.GetGreen(), colour.GetBlue(), 0); }
    void setClearColour(const float r, const float g, const float b, const float a) override { glClearColor(r, g, b, a); }

    [[nodiscard]] float getFrameTime() override { return m_DeltaTime; }
private:
    std::vector<const Object3D*> m_RegisteredObjects;
};
class VulkanRenderAPI : public RenderAPI {};

#endif //RENDERAPI_H
