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

	GLFWwindow* Window() const { return m_window; }

protected:
	/****** Methods ******/

	// Polls events
	inline virtual void PollEvents() { glfwPollEvents(); }
	// Swaps screen buffers
	inline virtual void SwapBuffers() { glfwSwapBuffers(m_window); }

	/****** Data ******/

	GLFWwindow* m_window;
	GLboolean m_resizable;
}; 

class MGLRenderer : public MGLContext {
public:
	MGLRenderer() :MGLContext(){};
	~MGLRenderer();

	virtual void RenderScene() = 0;
	virtual void InitGL() override;
	static void ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height);

protected:

	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_modelMatrix;

	MGLCamera* m_camera;
};