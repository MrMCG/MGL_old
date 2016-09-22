#pragma once
#include "stdafx.h"

class MGLMaterial {
public:
	MGLMaterial();
	~MGLMaterial();

	MGLMaterial(const MGLMaterial&) = delete;
	MGLMaterial(const MGLMaterial&&) = delete;
	MGLMaterial& operator=(const MGLMaterial&) = delete;
	MGLMaterial& operator=(const MGLMaterial&&) = delete;

private:
	glm::vec3 vecKa = glm::vec3(1.0f);
	glm::vec3 vecKd = glm::vec3(1.0f);
	glm::vec3 vecKs = glm::vec3(1.0f);
	GLfloat exponentNs = 1.0f;

	GLuint textureKa = 0;
	GLuint textureKd = 0;
};
