#pragma once
#include "stdafx.h"

#include "MGLMouse.h"
#include "MGLKeyboard.h"

#define MGL_WINDOWTYPE_WINDOWED 1
#define MGL_WINDOWTYPE_FULLSCREEN 2
#define MGL_WINDOWTYPE_BORDERLESS 3

class MGLWindow {
public:

	MGLWindow();

	GLint ShouldClose() { return glfwWindowShouldClose(window); }
	void CloseWindow() { glfwSetWindowShouldClose(window, GL_TRUE); }

	// TODO make setters read from file
	void SetGLVersion(GLuint major, GLuint minor);
	void SetWindowSize(GLuint newWidth, GLuint newHeight);
	void SetResizable(GLboolean resizable);
	void SetRefreshRate(GLboolean refresh);
	void SetSamples(GLuint samples);
	void SetWindowType(MGLenum type);
	void SetInFocus(GLboolean focus);

	GLFWwindow* GetGLFWWindow() { return window; }
	GLboolean GetResizable() { return isResizable; }
	GLboolean GetInFocus() { return inFocus; }
	GLuint GetWindowType() { return windowType; }
	GLuint GetWidth() { return width; }
	GLuint GetHeight() { return height; }
	GLuint GetRefreshRate() { return refreshRate; }
	GLuint GetSamples() { return samples; }

protected:

	static void ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height);

	virtual void HandleResize(GLuint width, GLuint height);
	virtual void SetGLOptions();

private:

	GLboolean GenerateWindow();
	void SetGLFWWindowContext();
	void InitGLEW();
	void InitWindowHints();
	void InitWindow();

	void MakeWindowFullscreen();
	void MakeWindowWindowed();
	void MakeWindowBorderless();

	// Set callback on window resize (if m_resizable=true and window!=NULL)
	void SetResizeCallback(GLFWwindowsizefun func);

	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;

	GLboolean isResizable = GL_FALSE;
	GLboolean inFocus = GL_TRUE;
	std::string title = "MGL Window!";

	MGLenum windowType = MGL_WINDOWTYPE_WINDOWED;

	GLuint glVersionMajor = 3;
	GLuint glVersionMinor = 3;
	GLuint width = 800;
	GLuint height = 600;
	GLuint refreshRate = 60;
	GLuint samples = 0;

	
};
