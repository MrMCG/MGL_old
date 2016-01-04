#include "stdafx.h"
#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "SOIL.lib")

// images: http://opengameart.org/content/50-free-textures-4-normalmaps
// http://bgfons.com/upload/stars_texture2944.jpg

class MGLContext {
public:
	// Defaults: MGL(3,3,true)
	MGLContext();
	MGLContext(GLuint major, GLuint minor, GLboolean resizable = true);
	virtual ~MGLContext();

	// Render loop
	virtual void RenderScene() = 0;
	
	// Init openGL options (use AFTER window is linked)
	virtual void InitGL();
	// Create a new window for use 
	virtual void CreateNewWindow(GLuint width, GLuint height, std::string title, MGLenum windowType, GLFWmonitor* monito = nullptr);
	// Should the window close
	virtual GLint ShouldClose() { return glfwWindowShouldClose(m_window); }
	// Set callback on window resize (if m_resizable=true and window!=NULL)
	void SetResizeCallback(GLFWwindowsizefun func);

	/* Setters */

	void SetWindow(GLFWwindow* win);
	
	/* Getters */

	GLFWwindow* GetWindow() const { return m_window; }

	
protected:
	/****** Methods ******/

	// Polls events
	inline virtual void PollEvents() { glfwPollEvents(); }
	// Swaps screen buffers
	inline virtual void SwapBuffers() { glfwSwapBuffers(m_window); }

	inline MGLContext* CastWindowToMGLContext(GLFWwindow* window) { 
		return static_cast<MGLContext*>(glfwGetWindowUserPointer(window));
	}

	// Handle keyboard input
	virtual void HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods);
	// Handle mouse button input
	virtual void HandleMouseButton(GLuint button, GLuint action, GLuint mods);
	// Handle window resize
	virtual void HandleResize(GLuint width, GLuint height);
	// Handle mouse position
	virtual void HandleMousePosition(GLdouble xPos, GLdouble yPos);
	// Handle mouse scroll
	virtual void HandleMouseScroll(GLdouble xOffset, GLdouble yOffset);
	// Handle window.mouse focus
	virtual void HandleMouseFocus(GLboolean focused);

	static void ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height);
	static void KeyInputCallBack(GLFWwindow* window, GLuint key, GLuint scancode, GLuint action, GLuint mods);
	static void MouseButtonCallBack(GLFWwindow* window, GLuint button, GLuint action, GLuint mods);
	static void MousePositionCallBack(GLFWwindow* window, GLdouble xPos, GLdouble yPos);
	static void MouseScrollCallBack(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset);
	static void MouseFocusCallBack(GLFWwindow* window, GLboolean focused);

	/****** Data ******/

	GLFWwindow* m_window;
	GLboolean m_resizable;
	GLuint m_width;
	GLuint m_height;
}; 

class MGLRenderer : public MGLContext {
public:
	MGLRenderer() :MGLContext(){};
	virtual ~MGLRenderer();

	virtual void InitGL() override;

	MGLCamera* GetCamera() { return m_camera; }
	MGLMouse* GetMouse() { return m_mouse; }

	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_modelMatrix;

protected:
	inline virtual void PollEvents();

	virtual void HandleResize(GLuint width, GLuint height) override;
	virtual void HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) override;
	virtual void HandleMousePosition(GLdouble xPos, GLdouble yPos) override;
	virtual void HandleMouseButton(GLuint button, GLuint action, GLuint mods) override;
	virtual void HandleMouseFocus(GLboolean focused) override;
	virtual void HandleMouseScroll(GLdouble xOffset, GLdouble yOffset);

	MGLCamera* m_camera;
	MGLKeyboard* m_keyboad;
	MGLMouse* m_mouse;
};