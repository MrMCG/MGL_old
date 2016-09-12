#include "stdafx.h"

#include "MGLRenderer.h"

MGLRenderer::MGLRenderer() : MGLContext() {
	camera = std::make_shared<MGLCamera>();
	MGLRenderer::InitInstances();

	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- INFO -----");
	WriteOGLInfo();
	WriteDefinesInfo();
	WriteWindowInfo();
	MGLH_Log->AddLog(MGL_LOG_MAIN, GL_FALSE, "----- INFO -----");
}

MGLRenderer::~MGLRenderer() {
	MGLH_Log->WriteToFile(MGL_LOG_FILENAME_MAIN, MGL_LOG_MAIN, GL_TRUE); // write main to file
	MGLH_Log->WriteToFile(MGL_LOG_FILENAME_ERROR, MGL_LOG_ERROR, GL_TRUE); // write errors to file

	MGLTexture::Release();
	MGLFileMGL::Release();
	MGLFileOBJ::Init();
	MGLCommonMeshes::Release();
	MGLLog::Release();
}

void MGLRenderer::CreateProjectionMatrix(const GLboolean orthographic) {
	projMatrix = orthographic ?
		glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 5000.0f) :
		glm::perspective(glm::radians(camera->GetZoom()), 
			(static_cast<GLfloat>(window->GetWidth()) / 
			 static_cast<GLfloat>(window->GetHeight())), 0.1f, 5000.0f);
}

void MGLRenderer::CreateViewMatrix() {
	viewMatrix = camera->BuildViewMatrix();
}

void MGLRenderer::InitInstances() {
	// ORDER IS IMPORTANT
	MGLTexture::Init();
	MGLFileMGL::Init();
	MGLFileOBJ::Init();
	MGLCommonMeshes::Init();
	MGLLog::Init();
}