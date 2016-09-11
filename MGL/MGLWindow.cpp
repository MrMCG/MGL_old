#include "stdafx.h"

#include "MGLWindow.h"
#include "MGLExceptions.h"
#include "MGLDebug.h"

MGLWindow::MGLWindow() {
	monitor = glfwGetPrimaryMonitor();
	GenerateWindow();
	InitGLEW();
	SetGLOptions();
	InitInput();
}

void MGLWindow::InitInput() {
	keyboadInput = new MGLKeyboard();
	mouseInput = new MGLMouse();
}

void MGLWindow::InitGLEW() {
	try {
		glewExperimental = GL_TRUE;
		MGLException_Init_GLEW::Test(glewInit());
	}
	catch (const MGLException& e) {
		MGLH_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());
		return;
	}
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "GLEW INIT: SUCCESS");
}

void MGLWindow::PollInput() {
	keyboadInput->RunKeys();
	mouseInput->RunKeys();
}

void MGLWindow::SetGLOptions() {
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

GLboolean MGLWindow::GenerateWindow() {
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "WINDOW INIT...");

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

void MGLWindow::InitWindowHints() {
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

	glfwSetKeyCallback(window, (GLFWkeyfun)this->KeyInputCallBack);
	glfwSetCursorEnterCallback(window, (GLFWcursorenterfun)this->MouseFocusCallBack);

	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)this->MouseButtonCallBack);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)this->MousePositionCallBack);
	glfwSetScrollCallback(window, (GLFWscrollfun)this->MouseScrollCallBack);

	SetResizeCallback((GLFWwindowsizefun)this->ResizeCallBack);
}

void MGLWindow::MakeWindowFullscreen() {
	window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
}

void MGLWindow::MakeWindowWindowed() {
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

void MGLWindow::MakeWindowBorderless() {
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);

	//delete mode;
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

void MGLWindow::SetResizeCallback(GLFWwindowsizefun func) {
	if (!isResizable || !window)
		return;

	glfwSetWindowSizeCallback(window, func);
}

void MGLWindow::ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height) {
	MGLWindow* game = static_cast<MGLWindow*>(glfwGetWindowUserPointer(window));
	game->HandleResize(width, height);
}

void MGLWindow::KeyInputCallBack(GLFWwindow* window, GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	MGLWindow* game = static_cast<MGLWindow*>(glfwGetWindowUserPointer(window));
	game->HandleKeyInput(key, scancode, action, mods);
}

void MGLWindow::MouseButtonCallBack(GLFWwindow* window, GLuint button, GLuint action, GLuint mods) {
	MGLWindow* game = static_cast<MGLWindow*>(glfwGetWindowUserPointer(window));
	game->HandleMouseButton(button, action, mods);
}

void MGLWindow::MousePositionCallBack(GLFWwindow* window, GLdouble xPos, GLdouble yPos) {
	MGLWindow* game = static_cast<MGLWindow*>(glfwGetWindowUserPointer(window));
	game->HandleMousePosition(xPos, yPos);
}

void MGLWindow::MouseScrollCallBack(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset) {
	MGLWindow* game = static_cast<MGLWindow*>(glfwGetWindowUserPointer(window));
	game->HandleMouseScroll(xOffset, yOffset);
}

void MGLWindow::MouseFocusCallBack(GLFWwindow* window, GLboolean focused) {
	MGLWindow* game = static_cast<MGLWindow*>(glfwGetWindowUserPointer(window));
	game->HandleMouseFocus(focused);
}

void MGLWindow::HandleResize(GLuint newWidth, GLuint newHeight) {
	SetWindowSize(newWidth, newHeight);
}

void MGLWindow::HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	keyboadInput->UpdateKey(key, mods, action);
}

void MGLWindow::HandleMouseButton(GLuint button, GLuint action, GLuint mods) {
	mouseInput->UpdateKey(button, mods, action);
}

void MGLWindow::HandleMousePosition(GLdouble xPos, GLdouble yPos) {
	if (inFocus)
		mouseInput->UpdatePosition((GLfloat)xPos, (GLfloat)yPos);
}

void MGLWindow::HandleMouseScroll(GLdouble xOffset, GLdouble yOffset) {
	mouseInput->UpdateScroll((GLfloat)xOffset, (GLfloat)yOffset);
	mouseInput->UpdateKey(MGL_SCROLLKEY_VALUE, 0, MGL_INPUT_SCROLLACTION);
}

void MGLWindow::HandleMouseFocus(GLboolean focused) {
	inFocus = focused;
	glfwSetInputMode(window, GLFW_CURSOR, focused ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
