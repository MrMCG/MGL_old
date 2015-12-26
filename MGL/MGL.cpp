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
	if (m_window)
		glfwDestroyWindow(m_window);

	m_window = win; 
	glfwMakeContextCurrent(m_window);
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

	try {
		switch (windowType) {
		case MGL_WINDOWTYPE_FULLSCREEN:
			m_window = glfwCreateWindow(width, height, title.c_str(), mon, nullptr);
			break;
		case MGL_WINDOWTYPE_WINDOWED:
			m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			break;
		case MGL_WINDOWTYPE_FULLWINDOWED: {
				if (width != 0 && height != 0)
					break;

				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

				glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

				if (width == 0 && height == 0)
					m_window = glfwCreateWindow(mode->width, mode->height, title.c_str(), mon, nullptr);
			}
			break;
		default:
			break;
		}

		MGLException_Init_WINDOW::IsSuccessful(m_window);
	}
	catch (const MGLException& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << " !! Creating Default Window !! " << std::endl;
		m_window = glfwCreateWindow(800, 600, "MGL: Default Window", nullptr, nullptr);
	}

	glfwMakeContextCurrent(m_window);

	std::cout << "WINDOW INIT: SUCCESS" << std::endl;
}

/***********************************/
/*********** MGLRenderer ***********/
/***********************************/

void MGLRenderer::InitGL() {
	MGLContext::InitGL();
	glfwWindowHint(GL_SAMPLES, 2);

	glViewport(0, 0,
		MGL::GetWindowInfo(m_window, MGL_WINDOWINFO_WIDTH),
		MGL::GetWindowInfo(m_window, MGL_WINDOWINFO_HEIGHT)
		);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	m_camera = nullptr;

	SetResizeCallback((GLFWwindowsizefun)this->ResizeCallBack);
}

void MGLRenderer::ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height) {
	glViewport(0, 0,
		MGL::GetWindowInfo(window, MGL_WINDOWINFO_WIDTH),
		MGL::GetWindowInfo(window, MGL_WINDOWINFO_HEIGHT)
		);
}

MGLRenderer::~MGLRenderer() {
	delete m_camera;
}