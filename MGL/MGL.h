#include "stdafx.h"
#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "SOIL.lib")

enum class WINDOWTYPE {FULLSCREEN, WINDOWED, FULLSCREEN_WINDOWED};

class MGLContext {
public:
	// Defaults: MGL(3,3,true)
	MGLContext();
	MGLContext(int major, int minor, bool resizable = true);
	virtual ~MGLContext();

	// Render loop
	virtual void RenderScene() = 0;

	// Init openGL options (use AFTER window is linked)
	virtual void InitGL();
	// Create a new window for use 
	virtual void CreateNewWindow(int width, int height, std::string title, WINDOWTYPE type);
	// Should the window close
	virtual int ShouldClose() { return glfwWindowShouldClose(m_window); }

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

	/****** Data ******/
	GLFWwindow* m_window;
	int m_windowWidth;
	int m_windowHeight;
}; 

class MGLRenderer : public MGLContext {
public:
	MGLRenderer() :MGLContext(){};
	~MGLRenderer(){};

	virtual void RenderScene() = 0;
	virtual void InitGL() override;
};