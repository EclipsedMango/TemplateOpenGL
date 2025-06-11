#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern Camera camera;

class Window {
public:
	static inline float lastX = 1200.0f / 2.0f;
	static inline float lastY = 800.0f / 2.0f;
	static inline bool firstMouse = true;
	static inline bool captureMouse = true;

	static GLFWwindow* initWindow(const int width, const int height, const char* title) {
        if (!glfwInit()) {
            std::cout << "Failed to initialize window\n";
            return nullptr;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_FLOATING, true);

        GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            std::cout << "Failed to create GLFW window\n";
            glfwTerminate();
            return nullptr;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cout << "Failed to initialize GLAD\n";
            return nullptr;
        }

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	    glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        return window;
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { camera.ProcessMouseScroll(static_cast<float>(yoffset)); }
	static void processInput(GLFWwindow *window, const float deltaTime) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { camera.ProcessKeyboard(FORWARD, deltaTime); }
	    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { camera.ProcessKeyboard(BACKWARD, deltaTime); }
	    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { camera.ProcessKeyboard(LEFT, deltaTime); }
	    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { camera.ProcessKeyboard(RIGHT, deltaTime); }
	    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
	    
	    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
	        if (captureMouse) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	        } else {
        		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	        }
    		captureMouse = !captureMouse;
	    }
	}

	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	    float xpos = static_cast<float>(xposIn);
	    float ypos = static_cast<float>(yposIn);

	    if (firstMouse) {
	        lastX = xpos;
	        lastY = ypos;
	        firstMouse = false;
	    }

	    float xoffset = xpos - lastX;
	    float yoffset = lastY - ypos;
		lastX = xpos;
	    lastY = ypos;

	    camera.ProcessMouseMovement(xoffset, yoffset);
	}
};

#endif