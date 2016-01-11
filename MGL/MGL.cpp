#include "stdafx.h"
#include "MGL.h"

/***************************/
/*********** MGL ***********/
/***************************/

MGLContext::MGLContext() : MGLContext(3, 3, true) {}
MGLContext::MGLContext(GLuint major, GLuint minor, GLboolean resizable) {
	MGLLog::Init();
	try {
		MGLException_Init_GLFW::IsSuccessful(glfwInit());
	}
	catch (const MGLException& e) {
		//std::cerr << e.what() << std::endl;
		MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // remove depreciated code
	glfwWindowHint(GLFW_RESIZABLE, resizable);

#ifdef MGLDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	m_inFocus = GL_TRUE;
	m_resizable = GL_TRUE;

	// init pointers
	m_window = nullptr;

	//std::cout << "GLFW INIT: SUCCESS" << std::endl;
	MGLLodHandle->AddLog(MGL_LOG_MAIN, GL_TRUE, "GLFW INIT: SUCCESS");

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
		//std::cerr << e.what() << std::endl;
		MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

		return;
	}

	//std::cout << "GLEW INIT: SUCCESS" << std::endl;
	MGLLodHandle->AddLog(MGL_LOG_MAIN, GL_TRUE, "GLFW INIT: SUCCESS");

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

	try {
		MGLException_Null::IsSuccessful(window);
	}
	catch (const MGLException& e) {
		//std::cerr << e.what() << " MGLContext::CreateNewWindow" << std::endl;
		//std::cerr << " !! Creating Default Window !! " << std::endl;
		MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%s", e.what(), " MGLContext::CreateNewWindow");
		MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, " !! Creating Default Window !! ");

		window = glfwCreateWindow(800, 600, "MGL: Default Window", nullptr, nullptr);
		m_width = 800;
		m_height = 600;
	}

	SetWindow(window);

	//std::cout << "WINDOW INIT: SUCCESS" << std::endl;
	MGLLodHandle->AddLog(MGL_LOG_MAIN, GL_TRUE, "WINDOW INIT: SUCCESS");

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
#ifdef MGLDEBUG
	std::cout << "HANDLERESIZE: " << width << " " << height << std::endl; // TEMP
#endif
}

void MGLContext::HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) {
#ifdef MGLDEBUG
	std::cout << "HANDLEKEYBOARD: " << key << " ";
	std::cout << scancode << " " << action << " " << mods << std::endl; // TEMP
#endif
}

void MGLContext::HandleMouseButton(GLuint button, GLuint action, GLuint mods) {
#ifdef MGLDEBUG
	std::cout << "HANDLEMOUSEBUTTON: " << button << " " << action << " " << mods << std::endl; // TEMP
#endif
}

void MGLContext::HandleMousePosition(GLdouble xPos, GLdouble yPos) {
#ifdef MGLDEBUG
	std::cout << "HANDLEMOUSEPOS: " << xPos << " " << yPos << std::endl; // TEMP
#endif
}

void MGLContext::HandleMouseScroll(GLdouble xOffset, GLdouble yOffset) {
#ifdef MGLDEBUG
	std::cout << "HANDLEMOUSESCROLL: " << xOffset << " " << yOffset << std::endl; // TEMP
#endif
}

void MGLContext::HandleMouseFocus(GLboolean focused) {
#ifdef MGLDEBUG
	std::cout << "HANDLEMOUSFOCUS: " << (focused ? "true" : "false") << std::endl; // TEMP
#endif
}

/***********************************/
/*********** MGLRenderer ***********/
/***********************************/

MGLRenderer::MGLRenderer() : MGLContext() {
	m_camera = nullptr;
	m_keyboad = new MGLKeyboard();
	m_mouse = new MGLMouse();
}

MGLRenderer::~MGLRenderer() {
	MGLLodHandle->WriteToFile(MGL_LOG_FILENAME_MAIN, GL_TRUE); // write main to file
	MGLLodHandle->WriteToFile(MGL_LOG_FILENAME_ERROR, GL_FALSE); // write errors to file

	delete m_camera;
	delete m_keyboad;
	delete m_mouse;

	MGLTexture::Release();
	MGLFile::Release();
	MGLCommonMeshes::Release();
	MGLLog::Release();
}

void MGLRenderer::InitGL() {
	MGLContext::InitGL();
	glfwWindowHint(GL_SAMPLES, 2);

	glViewport(0, 0, m_width, m_height);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	InitInstances();
}

void MGLRenderer::InitInstances() {
	// ORDER IS IMPORTANT
	MGLTexture::Init();
	MGLFile::Init();
	MGLCommonMeshes::Init();
	MGLLog::Init();
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
	m_inFocus = focused;
	if (focused)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void MGLRenderer::HandleMouseScroll(GLdouble xOffset, GLdouble yOffset) {
	MGLContext::HandleMouseScroll(xOffset, yOffset);
	m_mouse->UpdateScroll((GLfloat)xOffset, (GLfloat)yOffset);
}

