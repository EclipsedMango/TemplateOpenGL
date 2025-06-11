#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <../include/stb_image/stb_image.h>

#include "../headers/shader.h"
#include "../headers/camera.h"
#include "../headers/mesh.h"
#include "../headers/object3d.h"
#include "../headers/shaderManager.h"
#include "../headers/window.h"
#include "../headers/texture.h"

constexpr int windowWidth = 1200;
constexpr int windowHeight = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 0.3f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void countFrames();

int main() {
    // Window initialization and creation.
	GLFWwindow* window = Window::initWindow(windowWidth, windowHeight, "LearningOpenGL");
    if (!window) { 
        std::cout << "Null window\n";
    	return -1;
    }

    glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	ShaderManager shaderManager;

    Shader ourShader("../shaders/vertex.vs", "../shaders/fragment.fs");
	shaderManager.registerShader(&ourShader);

	Mesh cubeMesh;
	cubeMesh.vertices = {
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
	cubeMesh.indices = {
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

	Object3D cube = Object3D(&cubeMesh);

    Texture texture1("../assets/serble_logo.png", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, true);
    Texture texture2("../assets/aXR5PTgw.png", GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true);

	cube.textures.push_back(&texture1);
	cube.textures.push_back(&texture2);

	cube.bindMeshBuffers();

	// Uncomment for wireframe view.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.11f, 0.11f, 0.12f, 1.0f);

	while(!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		countFrames();
        Window::processInput(window, deltaTime);

		// Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Global shader values.
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
            static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
		shaderManager.injectGlobals(view, projection);

		cube.rotation = {0, (glfwGetTime() * 5.0f) * 50.0f, 0};
		cube.draw(ourShader);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cube.deleteBuffers();

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