#include "stdafx.h"

#include "MGLContext.h"

MGLContext::MGLContext() {	
	//FreeConsole();
	MGLLog::Init();
	InitGLFW();
}

MGLContext::~MGLContext() {
	MGLI_Log->WriteToFile(MGL_LOG_FILENAME_MAIN, MGL_LOG_MAIN, GL_TRUE); // write main to file
	MGLI_Log->WriteToFile(MGL_LOG_FILENAME_ERROR, MGL_LOG_ERROR, GL_TRUE); // write errors to file

	MGLTexture::Release();
	MGLFileLoaderMGL::Release();
	MGLFileOBJ::Release();
	MGLCommonMeshes::Release();
	MGLLog::Release();

	delete input;
	delete timer;
	delete window;

	glfwTerminate();
}

void MGLContext::InitMGL() {
	InitInstances();

	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- INFO -----");
	WriteOGLInfo();
	WriteDefinesInfo();
	WriteWindowInfo();
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- INFO -----");
}

void MGLContext::WriteDefinesInfo() {
	// get define states
	GLuint def_MGLDEBUG = 0;
	GLuint def_FILEVERSION = static_cast<GLuint>(MGL_FILE_CURRENTVERSION);
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
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "\tMGL DEFINES");

	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8s", "MGLDEBUG", def_MGLDEBUG ? "TRUE" : "FALSE");
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8s", "MGL_BUILD_CONFIG", def_BUILDCONFIG.c_str());
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8s", "MGL_USER_INCLUDE_TESTS", def_USER_TESTS ? "TRUE" : "FALSE");
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8s", "MGL_USER_INCLUDE_FILETC", def_USER_TC ? "TRUE" : "FALSE");
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "MGL_FILE_CURRENTVERSION", def_FILEVERSION);
}

void MGLContext::InitGLFW() {
	std::string message = "SUCCESS";
	try {
		MGLException_IsNotEqual::Test(glfwInit(), GL_TRUE);
	}
	catch (const MGLException& e) {
		message = e.what();
	}
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "GLFW INIT:"+message);
}

void MGLContext::InitInstances() {
	// ORDER IS IMPORTANT
	MGLTexture::Init();
	MGLFileLoaderMGL::Init();
	MGLFileOBJ::Init();
	MGLCommonMeshes::Init();
}

void MGLContext::BuildWindow() {
	BuildWindow(new MGLWindow());
}

void MGLContext::BuildWindow(MGLWindow* windo) {
	window = windo;
	input->AttatchInputToWindow(window);
}

GLdouble MGLContext::GetFrameDelta() const {
	return timer->GetTime();
}

void MGLContext::PollEvents() {
	timer->End();
	timer->Start();
	glfwPollEvents(); 
	input->PollInput();
}

void MGLContext::SwapBuffers() {
	glfwSwapBuffers(window->GetGLFWWindow());
}

void MGLContext::WriteOGLInfo() {
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "\tOGL INFO");

	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)));
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, reinterpret_cast<GLchar const*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

void MGLContext::WriteWindowInfo() {
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "\tWINDOW INFO");

	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i%-8i", "Resolution", window->GetWidth(), window->GetHeight());
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "Refresh", window->GetRefreshRate());
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "MSAA", window->GetSamples());
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "Type", window->GetWindowType());
}












