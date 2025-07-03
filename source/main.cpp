#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION

#include "../headers/Camera.h"
#include "../headers/Mesh.h"
#include "../headers/Object3d.h"
#include "../headers/RenderAPI.h"
#include "../headers/Shader.h"
#include "../headers/ShaderManager.h"
#include "../headers/Window.h"
#include "../headers/Texture.h"

constexpr int windowWidth = 1200;
constexpr int windowHeight = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 0.3f));

void countFrames();

int main() {
    // Window initialization and creation.
	if (!glfwInit()) {
		std::cout << "Failed to initialize window\n";
	}

	Window* window = new GLFWOpenGLWindow;

	window->createWindow({windowWidth, windowHeight}, "LearningOpenGL");
	window->setMouseInput(true);
	window->setActiveWindow();

	window->setCallBacks();

	const auto api = std::make_unique<OpenGlRenderAPI>();
	api->init();

	ShaderManager shaderManager;

    Shader ourShader("../shaders/vertex.vs", "../shaders/fragment.fs");
	Shader testShader("../shaders/vertex.vs", "../Shaders/fragment2.fs");

	shaderManager.registerShader(&testShader);
	shaderManager.registerShader(&ourShader);

	Mesh* cubeMesh = new Mesh();
	cubeMesh->vertices = {
		// Back face
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

		// Front face
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},

		// Left face
		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},

		// Right face
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},

		// Bottom face
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},

		// Top face
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
	};
	cubeMesh->indices = {
		// Back face
		0, 1, 2,  2, 3, 0,
		// Front face
		4, 5, 6,  6, 7, 4,
		// Left face
		8, 9,10, 10,11, 8,
		// Right face
		12,13,14, 14,15,12,
		// Bottom face
		16,17,18, 18,19,16,
		// Top face
		20,21,22, 22,23,20
	};

	cubeMesh->gpuBuffer = api->CreateGpuBuffer(cubeMesh->vertices, cubeMesh->indices);

	Object3D cube = Object3D(cubeMesh);
	Object3D lightCube = Object3D(cubeMesh);

    Texture texture1("../assets/serble_logo.png", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true);
    Texture texture2("../assets/aXR5PTgw.png", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true);

	cube.textures.push_back(&texture1);
	cube.textures.push_back(&texture2);

	cube.shader = &ourShader;
	lightCube.shader = &testShader;

	api->registerObject(&cube);
	api->registerObject(&lightCube);

	while(!window->shouldWindowClose()) {
		api->startDrawing();
		api->setClearColour(0.11f, 0.11f, 0.12f, 1.0f);

		float deltaTime = api->getFrameTime();

		countFrames();
        window->processInput(deltaTime, camera);

		// Global shader values.
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
            static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
		shaderManager.injectGlobals(view, projection);

		cube.rotation = {0, (glfwGetTime() * 5.0f) * 50.0f, 0};
		lightCube.position = {-2, 0, 0};

		api->drawRegisteredObjects();
		api->endDrawing(window);
    }

    glfwTerminate();
    return 0;
}

void countFrames() {
	static double prevTime = glfwGetTime();
	static int frames = 0;
	frames++;

	if (const double time = glfwGetTime(); time - prevTime >= 1.0) {
		std::cout << "FPS: " << frames << "\n";
		frames = 0;
		prevTime = time;
	}
}