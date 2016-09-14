#include "stdafx.h"

#include "MGLContext.h"

MGLContext::MGLContext() {	
	//FreeConsole();
	MGLLog::Init();
	InitGLFW();
}

MGLContext::~MGLContext() {
	MGLH_Log->WriteToFile(MGL_LOG_FILENAME_MAIN, MGL_LOG_MAIN, GL_TRUE); // write main to file
	MGLH_Log->WriteToFile(MGL_LOG_FILENAME_ERROR, MGL_LOG_ERROR, GL_TRUE); // write errors to file

	MGLTexture::Release();
	MGLFileMGL::Release();
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

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- INFO -----");
	WriteOGLInfo();
	WriteDefinesInfo();
	WriteWindowInfo();
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- INFO -----");
}

void MGLContext::WriteDefinesInfo() {
	// get define states
	GLuint def_MGLDEBUG = 0;
	GLuint def_FILEVERSION = static_cast<GLuint>(MGL_FILE_CURRENTVERSION);
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

void MGLContext::InitGLFW() {
	std::string message = "SUCCESS";
	try {
		MGLException_Init_GLFW::Test(glfwInit());
	}
	catch (const MGLException& e) {
		message = e.what();
	}
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "GLFW INIT:"+message);
}

void MGLContext::InitInstances() {
	// ORDER IS IMPORTANT
	MGLTexture::Init();
	MGLFileMGL::Init();
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
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "\tOGL INFO");

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, reinterpret_cast<GLchar const*>(glGetString(GL_VERSION)));
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, reinterpret_cast<GLchar const*>(glGetString(GL_VENDOR)));
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, reinterpret_cast<GLchar const*>(glGetString(GL_RENDERER)));
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, reinterpret_cast<GLchar const*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

void MGLContext::WriteWindowInfo() {
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "\tWINDOW INFO");

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i%-8i", "Resolution", window->GetWidth(), window->GetHeight());
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "Refresh", window->GetRefreshRate());
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "MSAA", window->GetSamples());
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "%-32s%-8i", "Type", window->GetWindowType());
}












