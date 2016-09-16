#include "stdafx.h"

#include "MGLWindow.h"
#include "MGLExceptions.h"
#include "MGLLog.h"

MGLWindow::MGLWindow() {
	monitor = glfwGetPrimaryMonitor();
	GenerateWindow();
	InitGLEW();
	MGLWindow::SetGLOptions();
}

MGLWindow::~MGLWindow() {
	monitor = nullptr;
	glfwDestroyWindow(window);
}

void MGLWindow::InitGLEW() {
	try {
		glewExperimental = GL_TRUE;
		MGLException_Init_GLEW::Test(glewInit());
	}
	catch (const MGLException& e) {
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());
		return;
	}
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "GLEW INIT: SUCCESS");
}

void MGLWindow::SetGLOptions() {
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

GLboolean MGLWindow::GenerateWindow() {
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "WINDOW INIT...");

	if (window)
		return GL_FALSE;
	
	InitWindowHints();
	InitWindow();
	SetGLFWWindowContext();
	
	return GL_TRUE;
}

void MGLWindow::InitWindow() {
	switch (windowType) {
	case MGL_WINDOWTYPE_FULLSCREEN:
		MakeWindowFullscreen(); break;
	case MGL_WINDOWTYPE_WINDOWED:
		MakeWindowWindowed(); break;
	case MGL_WINDOWTYPE_BORDERLESS:
		MakeWindowBorderless(); break;
	default:
		window = glfwCreateWindow(800, 600, "MGL Default Window!", nullptr, nullptr);
	}
}

void MGLWindow::InitWindowHints() const {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // remove depreciated OGL code
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, isResizable);
	glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);
	glfwWindowHint(GLFW_SAMPLES, samples);
}

void MGLWindow::SetGLFWWindowContext() {
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);

	SetResizeCallback(reinterpret_cast<GLFWwindowsizefun>(this->ResizeCallBack));
}

void MGLWindow::MakeWindowFullscreen() {
	window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
}

void MGLWindow::MakeWindowWindowed() {
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

void MGLWindow::MakeWindowBorderless() {
	auto mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);
}

void MGLWindow::SetGLVersion(GLuint major, GLuint minor) {
	glVersionMajor = major;
	glVersionMinor = minor;
}

void MGLWindow::SetWindowSize(GLuint newWidth, GLuint newHeight) {
	glfwSetWindowSize(window, newWidth, newHeight);
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
}

void MGLWindow::SetResizable(GLboolean resizable) {
	isResizable = resizable;
}

void MGLWindow::SetRefreshRate(GLboolean refresh) {
	refreshRate = refresh;
}

void MGLWindow::SetSamples(GLuint newSamples) {
	samples = newSamples;
}

void MGLWindow::SetWindowType(MGLenum type) {
	windowType = type;
}

void MGLWindow::SetInFocus(GLboolean focus) {
	inFocus = focus;
}

void MGLWindow::ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height) {
	MGLWindow* game = static_cast<MGLWindow*>(glfwGetWindowUserPointer(window));
	game->HandleResize(width, height);
}

void MGLWindow::SetResizeCallback(GLFWwindowsizefun func) const {
	if (!isResizable || !window)
		return;

	glfwSetWindowSizeCallback(window, func);
}

void MGLWindow::HandleResize(GLuint newWidth, GLuint newHeight) {
	SetWindowSize(newWidth, newHeight);
}

