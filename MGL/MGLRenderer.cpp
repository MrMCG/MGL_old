#include "stdafx.h"

#include "MGLRenderer.h"

MGLRenderer::~MGLRenderer() {
	delete camera;

	meshes->DeleteAll();
	delete meshes;
}

void MGLRenderer::InitMGL() {
	MGLContext::InitMGL();
	meshes = new MGLDataMap<MGLMesh>(MGLMeshGenerator::Triangle());
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
