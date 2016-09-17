#pragma once
#include "stdafx.h"

#include "MGLLog.h"

/*
 * TODO: create window config file for loading options
 */

class MGLWindow {
public:

	MGLWindow();
	virtual ~MGLWindow();

	GLint ShouldClose() const { return glfwWindowShouldClose(window); }
	void CloseWindow() const { glfwSetWindowShouldClose(window, GL_TRUE); }

	void SetGLVersion(GLuint major, GLuint minor);
	void SetWindowSize(GLuint newWidth, GLuint newHeight);
	void SetResizable(GLboolean resizable);
	void SetRefreshRate(GLboolean refresh);
	void SetSamples(GLuint samples);
	void SetWindowType(MGLenum type);
	void SetInFocus(GLboolean focus);

	GLFWwindow* GetGLFWWindow() const { return window; }
	GLboolean GetResizable() const { return isResizable; }
	GLboolean GetInFocus() const { return inFocus; }
	GLuint GetWindowType() const { return windowType; }
	GLuint GetWidth() const { return width; }
	GLuint GetHeight() const { return height; }
	GLuint GetRefreshRate() const { return refreshRate; }
	GLuint GetSamples() const { return samples; }

	MGLWindow(const MGLWindow& other) = delete;
	MGLWindow(const MGLWindow&& other) = delete;
	MGLWindow& operator=(const MGLWindow& other) = delete;
	MGLWindow& operator=(const MGLWindow&& other) = delete;

	enum WindowType { Windowed, Fullscreen, Borderless };

protected:

	static void ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height);

	virtual void HandleResize(GLuint width, GLuint height);
	virtual void SetGLOptions();
	virtual void InitWindowHints() const;

private:

	GLboolean GenerateWindow();
	void SetGLFWWindowContext();
	static void InitGLEW();
	void InitWindow();

	void MakeWindowFullscreen();
	void MakeWindowWindowed();
	void MakeWindowBorderless();

	// Set callback on window resize (if m_resizable=true and window!=NULL)
	void SetResizeCallback(GLFWwindowsizefun func) const;

	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;

	GLboolean isResizable = GL_FALSE;
	GLboolean inFocus = GL_TRUE;
	std::string title = "MGL Window!";

	GLuint windowType = Windowed;

	GLuint glVersionMajor = 3;
	GLuint glVersionMinor = 3;
	GLuint width = 800;
	GLuint height = 600;
	GLuint refreshRate = 60;
	GLuint samples = 0;

	
};
