#include "stdafx.h"
#include "MGL.h"

/***************************/
/*********** MGL ***********/
/***************************/

MGLContext::MGLContext() {

	MGLLog::Init();
	try {
		MGLException_Init_GLFW::Test(glfwInit());
	}
	catch (const MGLException& e) {
		MGLH_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());
		return;
	}

	// init pointers
	window = new MGLWindow();

	//std::cout << "GLFW INIT: SUCCESS" << std::endl;
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "GLFW INIT: SUCCESS");

}

MGLContext::~MGLContext() {
	delete window;
	glfwTerminate();
}

void MGLContext::WriteDefinesInfo() {
	// get define states
	GLuint def_MGLDEBUG = 0;
	GLuint def_FILEVERSION = (GLuint)MGL_FILE_CURRENTVERSION;
	GLuint def_LOGLINESIZE = MGL_LOG_MAXLINESIZE;
	GLuint def_LOGTOTALSIZE = MGL_LOG_MAXLOGSIZE;
	std::string def_BUILDCONFIG = "unknown";
	GLuint def_USER_TC = 0;
	GLuint def_USER_TESTS = 0;

	// define logic
#ifdef MGLDEBUG
	def_MGLDEBUG = 1;
#endif // MGLDEBUG
#ifdef MGL_USER_INCLUDE_TESTS
	def_USER_TESTS = 1;
#endif // MGL_USER_INCLUDE_TESTS
#ifdef MGL_USER_INCLUDE_FILETC
	def_USER_TC = 1;
#endif // MGL_USER_INCLUDE_FILETC
#ifdef MGL_BUILD_CONFIG
	def_BUILDCONFIG = MGL_BUILD_CONFIG;
#endif // MGL_BUILD_CONFIG

	// log variables
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "\tMGL DEFINES");

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8s", "MGLDEBUG", def_MGLDEBUG ? "TRUE" : "FALSE");
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8s", "MGL_BUILD_CONFIG", def_BUILDCONFIG.c_str());
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8s", "MGL_USER_INCLUDE_TESTS", def_USER_TESTS ? "TRUE" : "FALSE");
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8s", "MGL_USER_INCLUDE_FILETC", def_USER_TC ? "TRUE" : "FALSE");
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "MGL_FILE_CURRENTVERSION", def_FILEVERSION);
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "MGL_LOG_MAXLINESIZE", def_LOGLINESIZE);
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "MGL_LOG_MAXLOGSIZE", def_LOGTOTALSIZE);
}

void MGLContext::WriteOGLInfo() {
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "\tOGL INFO");

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, (GLchar*)glGetString(GL_VERSION));
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, (GLchar*)glGetString(GL_VENDOR));
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, (GLchar*)glGetString(GL_RENDERER));
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, (GLchar*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void MGLContext::WriteWindowInfo() {
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "\tWINDOW INFO");

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i%-8i", "Resolution", window->GetWidth(), window->GetHeight());
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "Refresh", window->GetRefreshRate());
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "MSAA", window->GetSamples());
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "Type", window->GetWindowType());
}

/***********************************/
/*********** MGLRenderer ***********/
/***********************************/

MGLRenderer::MGLRenderer() : MGLContext() {
	m_camera = new MGLCamera();
	InitInstances();

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- INFO -----");
	WriteOGLInfo();
	WriteDefinesInfo();
	WriteWindowInfo();
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- INFO -----");
}

MGLRenderer::~MGLRenderer() {
	MGLH_Log->WriteToFile(MGL_LOG_FILENAME_MAIN, MGL_LOG_MAIN, GL_TRUE); // write main to file
	MGLH_Log->WriteToFile(MGL_LOG_FILENAME_ERROR, MGL_LOG_ERROR, GL_TRUE); // write errors to file

	delete m_camera;

	MGLTexture::Release();
	MGLFileMGL::Release();
	MGLFileOBJ::Init();
	MGLCommonMeshes::Release();
	MGLLog::Release();
}

void MGLRenderer::CreateProjectionMatrix(const GLboolean orthographic) {
	m_projMatrix = orthographic ?
		glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 5000.0f) :
		glm::perspective(glm::radians(m_camera->GetZoom()), ((GLfloat)window->GetWidth() / (GLfloat)window->GetHeight()), 0.1f, 5000.0f);	
}

void MGLRenderer::CreateViewMatrix() {
	m_viewMatrix = m_camera->BuildViewMatrix();
}

void MGLRenderer::InitInstances() {
	// ORDER IS IMPORTANT
	MGLTexture::Init();
	MGLFileMGL::Init();
	MGLFileOBJ::Init();
	MGLCommonMeshes::Init();
	MGLLog::Init();
}










