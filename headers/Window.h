#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MathHeaders/Vector2.h"
#include "Camera.h"

extern Camera camera;

class Window {
public:
	Window() = default;
	virtual ~Window() = default;

	virtual void createWindow(emc::Vector2 bounds, const std::string& title) = 0;

	virtual void setTitle(const std::string& title) = 0;
	[[nodiscard]] virtual std::string getTitle() const = 0;

	virtual void setBounds(emc::Vector2 bounds) = 0;
	virtual void setBounds(float x, float y) = 0;
	[[nodiscard]] virtual emc::Vector2 getBounds() const = 0;

	virtual void setActiveWindow() = 0;
	virtual void setCallBacks() = 0;

	virtual void setMouseInput(bool captured) = 0;
	virtual void processInput(float deltaTime, Camera& camera) = 0;

	virtual void swapBuffers() = 0;

	virtual bool shouldWindowClose() = 0;
};

class GLFWOpenGLWindow final : public Window {
public:
	GLFWOpenGLWindow() = default;
	~GLFWOpenGLWindow() override {
		if (m_window) {
			glfwDestroyWindow(m_window);
		}
	}

	void createWindow(const emc::Vector2 bounds, const std::string& title) override {
		// should be done before create window (create window should assume this has already been done)
		// this should not be done everytime a window is created.
		// if (!glfwInit()) {
		// 	std::cout << "Failed to initialize window\n";
		// 	return;
		// }

		_setGlfwWindowHints();

		m_title = title;
		m_window = glfwCreateWindow(static_cast<int>(bounds.x), static_cast<int>(bounds.y), title.c_str(), nullptr, nullptr);

		if (!m_window) {
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwSetWindowUserPointer(m_window, this);
	}

	bool shouldWindowClose() override {
		return glfwWindowShouldClose(m_window);
	}

	void setActiveWindow() override {
		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(0);
	}

	void setCallBacks() override {
		glfwSetFramebufferSizeCallback(m_window, _framebuffer_size_callback_static);
		glfwSetCursorPosCallback(m_window, _mouse_callback_static);
		glfwSetScrollCallback(m_window, _scroll_callback_static);
	}

	void setMouseInput(const bool captured) override {
		m_captureMouse = captured;
		if (m_captureMouse) {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void processInput(const float deltaTime, Camera& camera) override {
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) { camera.ProcessKeyboard(FORWARD, deltaTime); }
		if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) { camera.ProcessKeyboard(BACKWARD, deltaTime); }
		if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) { camera.ProcessKeyboard(LEFT, deltaTime); }
		if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) { camera.ProcessKeyboard(RIGHT, deltaTime); }
		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(m_window, true); }
		if (glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_PRESS) { setMouseInput(!m_captureMouse); }
	}

	void swapBuffers() override {
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void setTitle(const std::string &title) override {
		m_title = title;
		glfwSetWindowTitle(m_window, m_title.c_str());
	}

	[[nodiscard]] std::string getTitle() const override {
		return m_title;
	}

	void setBounds(const emc::Vector2 bounds) override {
		setBounds(bounds.x, bounds.y);
	}

	void setBounds(const float x, const float y) override {
		glfwSetWindowSize(m_window, static_cast<int>(x), static_cast<int>(y));
	}

	[[nodiscard]] emc::Vector2 getBounds() const override {
		int x, y;
		glfwGetWindowSize(m_window, &x, &y);
		return {static_cast<float>(x), static_cast<float>(y)};
	}

private:
	GLFWwindow* m_window = nullptr;
	std::string m_title;

	float m_lastX = 1200.0f / 2.0f;
	float m_lastY = 800.0f / 2.0f;
	bool m_firstMouse = true;
	bool m_captureMouse = true;

	void _scroll_callback(GLFWwindow* window, double yoffset, Camera& camera) {
		camera.ProcessMouseScroll(static_cast<float>(yoffset));
	}

	void _mouse_callback(double xposIn, double yposIn, Camera& camera) {
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (m_firstMouse) {
			m_lastX = xpos;
			m_lastY = ypos;
			m_firstMouse = false;
		}

		float xoffset = xpos - m_lastX;
		float yoffset = m_lastY - ypos;
		m_lastX = xpos;
		m_lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	static void _framebuffer_size_callback_static(GLFWwindow* window, const int width, const int height) { glViewport(0, 0, width, height); }
	static void _scroll_callback_static(GLFWwindow* window, double xoffset, double yoffset) {
		auto* self = static_cast<GLFWOpenGLWindow*>(glfwGetWindowUserPointer(window));
		if (self) {
			self->_scroll_callback(window, yoffset, camera);
		}
	}

	static void _mouse_callback_static(GLFWwindow* window, double xposIn, double yposIn) {
		auto* self = static_cast<GLFWOpenGLWindow*>(glfwGetWindowUserPointer(window));
		if (self) {
			self->_mouse_callback(xposIn, yposIn, camera);
		}
	}

	static void _setGlfwWindowHints() {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_FLOATING, true);
	}
};

class GLFWVulkanWindow : public Window {};

#endif