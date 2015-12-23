#include "stdafx.h"
#include "MGL.h"

/***************************/
/*********** MGL ***********/
/***************************/

MGLContext::MGLContext() : MGLContext(3, 3, true) {}
MGLContext::MGLContext(int major, int minor, bool resizable) {
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

void MGLContext::CreateNewWindow(int width, int height, std::string title, WINDOWTYPE type) {
	if (m_window)
		glfwDestroyWindow(m_window);

	try {
		switch (type) {
		case WINDOWTYPE::FULLSCREEN:
			m_window = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
			break;
		case WINDOWTYPE::WINDOWED:
			m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			break;
		case WINDOWTYPE::FULLSCREEN_WINDOWED: {
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

				glfwWindowHint(GLFW_RED_BITS, mode->redBits);
				glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
				glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

				m_window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
			}
			break;
		default:
			m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			break;
		}

		MGLException_Init_WINDOW::IsSuccessful(
			m_window);
	}
	catch (const MGLException& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

	glfwMakeContextCurrent(m_window);

	m_windowWidth = width;
	m_windowHeight = height;

	std::cout << "WINDOW INIT: SUCCESS" << std::endl;
}

/***********************************/
/*********** MGLRenderer ***********/
/***********************************/
void MGLRenderer::InitGL() {
	MGLContext::InitGL();
	glfwWindowHint(GL_SAMPLES, 2);

	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}