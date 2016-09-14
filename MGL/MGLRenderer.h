#pragma once
#include "stdafx.h"

#include "MGLContext.h"
#include "MGLCamera.h"

class MGLRenderer : public MGLContext {
public:
	MGLRenderer() {}
	virtual ~MGLRenderer();

	MGLCamera* GetCamera() const { return camera; }

	/****** TEMP ******/
	glm::mat4 projMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelMatrix;
	/****** TEMP ******/

protected:

	virtual void CreateViewMatrix();
	virtual void CreateProjectionMatrix(const GLboolean orthographic = GL_FALSE);

	MGLCamera* camera = new MGLCamera();
};