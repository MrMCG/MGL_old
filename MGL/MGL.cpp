#include "stdafx.h"
#include "MGL.h"

/***************************/
/*********** MGL ***********/
/***************************/

MGLContext::MGLContext() : MGLContext(3, 3, true) {}
MGLContext::MGLContext(GLuint major, GLuint minor, GLboolean resizable) {
	try {
		MGLException_Init_GLFW::IsSuccessful(glfwInit());
	}
	catch (const MGLException& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	m_resizable = GL_TRUE;

	// init pointers
	m_window = nullptr;

	std::cout << "GLFW INIT: SUCCESS" << std::endl;
}

MGLContext::~MGLContext() {
	if (m_window)
		glfwDestroyWindow(m_window);

	glfwTerminate();
}

void MGLContext::InitGL() {
	try {
		glewExperimental = GL_TRUE;
		MGLException_Init_GLEW::IsSuccessful(glewInit());
	}
	catch (const MGLException& e) {
		std::cerr << e.what() << std::endl;

		return;
	}

	std::cout << "GLEW INIT: SUCCESS" << std::endl;
}

void MGLContext::SetWindow(GLFWwindow* win) {
	m_window = win; 
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	glfwSetKeyCallback(m_window, (GLFWkeyfun)this->KeyInputCallBack);
	glfwSetCursorEnterCallback(m_window, (GLFWcursorenterfun)this->MouseFocusCallBack);

	glfwSetMouseButtonCallback(m_window, (GLFWmousebuttonfun)this->MouseButtonCallBack);
	glfwSetCursorPosCallback(m_window, (GLFWcursorposfun)this->MousePositionCallBack);
	glfwSetScrollCallback(m_window, (GLFWscrollfun)this->MouseScrollCallBack);
	
	SetResizeCallback((GLFWwindowsizefun)this->ResizeCallBack);
}

void MGLContext::SetResizeCallback(GLFWwindowsizefun func) {
	if (!m_resizable || !m_window)
		return;

	glfwSetWindowSizeCallback(m_window, func);
}

void MGLContext::CreateNewWindow(GLuint width, GLuint height, std::string title, MGLenum windowType, GLFWmonitor* monitor) {
	if (m_window)
		glfwDestroyWindow(m_window);

	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	if (monitor)
		mon = monitor;

	m_width = width;
	m_height = height;

	GLFWwindow* window = nullptr;

	try {
		switch (windowType) {
		case MGL_WINDOWTYPE_FULLSCREEN:
			window = glfwCreateWindow(width, height, title.c_str(), mon, nullptr);
			break;
		case MGL_WINDOWTYPE_WINDOWED:
			window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			break;
		case MGL_WINDOWTYPE_FULLWINDOWED: {
				if (width != 0 && height != 0)
					break;

				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

				glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

				if (width == 0 && height == 0) {
					window = glfwCreateWindow(mode->width, mode->height, title.c_str(), mon, nullptr);
					m_width = mode->width;
					m_height = mode->height;
				}
			}
			break;
		default:
			break;
		}

		MGLException_Init_WINDOW::IsSuccessful(window);
	}
	catch (const MGLException& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << " !! Creating Default Window !! " << std::endl;
		window = glfwCreateWindow(800, 600, "MGL: Default Window", nullptr, nullptr);
		m_width = 800;
		m_height = 600;
	}

	SetWindow(window);

	std::cout << "WINDOW INIT: SUCCESS" << std::endl;
}

void MGLContext::ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height) {
	MGLContext* game = static_cast<MGLContext*>(glfwGetWindowUserPointer(window));
	game->HandleResize(width, height);
}

void MGLContext::KeyInputCallBack(GLFWwindow* window, GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	MGLContext* game = static_cast<MGLContext*>(glfwGetWindowUserPointer(window));
	game->HandleKeyInput(key, scancode, action, mods);
}

void MGLContext::MouseButtonCallBack(GLFWwindow* window, GLuint button, GLuint action, GLuint mods) {
	MGLContext* game = static_cast<MGLContext*>(glfwGetWindowUserPointer(window));
	game->HandleMouseButton(button, action, mods);
}

void MGLContext::MousePositionCallBack(GLFWwindow* window, GLdouble xPos, GLdouble yPos) {
	MGLContext* game = static_cast<MGLContext*>(glfwGetWindowUserPointer(window));
	game->HandleMousePosition(xPos, yPos);
}

void MGLContext::MouseScrollCallBack(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset) {
	MGLContext* game = static_cast<MGLContext*>(glfwGetWindowUserPointer(window));
	game->HandleMouseScroll(xOffset, yOffset);
}

void MGLContext::MouseFocusCallBack(GLFWwindow* window, GLboolean focused) {
	MGLContext* game = static_cast<MGLContext*>(glfwGetWindowUserPointer(window));
	game->HandleMouseFocus(focused);
}

void MGLContext::HandleResize(GLuint width, GLuint height) {
#ifdef _DEBUG
	std::cout << "HANDLERESIZE: " << width << " " << height << std::endl;
#endif
}

void MGLContext::HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) {
#ifdef _DEBUG
	std::cout << "HANDLEKEYBOARD: " << key << " ";
	std::cout << scancode << " " << action << " " << mods << std::endl;
#endif
}

void MGLContext::HandleMouseButton(GLuint button, GLuint action, GLuint mods) {
#ifdef _DEBUG
	std::cout << "HANDLEMOUSEBUTTON: " << button << " " << action << " " << mods << std::endl;
#endif
}

void MGLContext::HandleMousePosition(GLdouble xPos, GLdouble yPos) {
#ifdef _DEBUG
	std::cout << "HANDLEMOUSEPOS: " << xPos << " " << yPos << std::endl;
#endif
}

void MGLContext::HandleMouseScroll(GLdouble xOffset, GLdouble yOffset) {
#ifdef _DEBUG
	std::cout << "HANDLEMOUSESCROLL: " << xOffset << " " << yOffset << std::endl;
#endif
}

void MGLContext::HandleMouseFocus(GLboolean focused) {
#ifdef _DEBUG
	std::cout << "HANDLEMOUSFOCUS: " << (focused ? "true" : "false") << std::endl;
#endif
}

/***********************************/
/*********** MGLRenderer ***********/
/***********************************/

void MGLRenderer::InitGL() {
	MGLContext::InitGL();
	glfwWindowHint(GL_SAMPLES, 2);

	glViewport(0, 0, m_width, m_height);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	m_camera = nullptr;
	m_keyboad = new MGLKeyboard();
	m_mouse = new MGLMouse();
}

void MGLRenderer::PollEvents() {
	MGLContext::PollEvents(); 
	m_keyboad->RunKeys(); 
	m_mouse->RunKeys();
}

void MGLRenderer::HandleResize(GLuint width, GLuint height) {
	MGLContext::HandleResize(width, height);
	glViewport(0, 0, width, height);
	m_width = width;
	m_height = height;
}

void MGLRenderer::HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	MGLContext::HandleKeyInput(key, scancode, action, mods);
	m_keyboad->UpdateKey(key, action);
	m_keyboad->UpdateMod(mods);
}

void MGLRenderer::HandleMousePosition(GLdouble xPos, GLdouble yPos) {
	MGLContext::HandleMousePosition(xPos, yPos);
	m_mouse->UpdatePosition((GLfloat)xPos, (GLfloat)yPos);
}

void MGLRenderer::HandleMouseButton(GLuint button, GLuint action, GLuint mods) {
	MGLContext::HandleMouseButton(button, action, mods);
	m_mouse->UpdateKey(button, action);
	m_mouse->UpdateMod(mods);
}

void MGLRenderer::HandleMouseFocus(GLboolean focused) {
	MGLContext::HandleMouseFocus(focused);
	if (focused)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void MGLRenderer::HandleMouseScroll(GLdouble xOffset, GLdouble yOffset) {
	MGLContext::HandleMouseScroll(xOffset, yOffset);
	m_mouse->UpdateScroll(xOffset, yOffset);
}

MGLRenderer::~MGLRenderer() {
	delete m_camera;
	delete m_keyboad;
	delete m_mouse;
}