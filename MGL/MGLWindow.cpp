#include "stdafx.h"

#include "MGLWindow.h"
#include "MGLExceptions.h"
#include "MGLDebug.h"

MGLWindow::MGLWindow() {
	monitor = glfwGetPrimaryMonitor();
	GenerateNewGLFWWindow();
	InitGL();
	InitInput();
}

void MGLWindow::InitInput() {
	keyboadInput = new MGLKeyboard();
	mouseInput = new MGLMouse();
}

void MGLWindow::InitGL() {
	try {
		glewExperimental = GL_TRUE;
		MGLException_Init_GLEW::Test(glewInit());
	}
	catch (const MGLException& e) {
		//std::cerr << e.what() << std::endl;
		MGLH_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

		return;
	}

	//std::cout << "GLEW INIT: SUCCESS" << std::endl;
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "GLEW INIT: SUCCESS");

	glfwWindowHint(GL_SAMPLES, samples);

	glViewport(0, 0, width, height);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void MGLWindow::PollInput() {
	keyboadInput->RunKeys();
	mouseInput->RunKeys();
}

void MGLWindow::GenerateNewGLFWWindow() {
	if (window)
		glfwDestroyWindow(window);

	InitWindowHints();

	switch (windowType) {
	case MGL_WINDOWTYPE_FULLSCREEN:
		MakeWindowFullscreen(); break;
	case MGL_WINDOWTYPE_WINDOWED:
		MakeWindowWindowed(); break;
	case MGL_WINDOWTYPE_FULLWINDOWED: 
		MakeWindowBorderless(); break;
	default:
		window = glfwCreateWindow(800, 600, "MGL Default Window!", nullptr, nullptr);
	}

	SetGLFWWindowContext();

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "WINDOW INIT...");
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
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);

	//delete mode;
}

void MGLWindow::SetGLVersion(GLuint major, GLuint minor) {
	glVersionMajor = major;
	glVersionMinor = minor;
}

void MGLWindow::SetSize(GLuint newWidth, GLuint newHeight) {
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
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
}

void MGLWindow::HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	keyboadInput->UpdateKey(key, action);
	keyboadInput->UpdateMod(mods);
}

void MGLWindow::HandleMouseButton(GLuint button, GLuint action, GLuint mods) {
	mouseInput->UpdateKey(button, action);
	mouseInput->UpdateMod(mods);
}

void MGLWindow::HandleMousePosition(GLdouble xPos, GLdouble yPos) {
	mouseInput->UpdatePosition((GLfloat)xPos, (GLfloat)yPos);
}

void MGLWindow::HandleMouseScroll(GLdouble xOffset, GLdouble yOffset) {
	mouseInput->UpdateScroll((GLfloat)xOffset, (GLfloat)yOffset);
}

void MGLWindow::HandleMouseFocus(GLboolean focused) {
	inFocus = focused;
	if (focused)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
