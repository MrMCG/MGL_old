#pragma once
#include "stdafx.h"

class MGLInputItem {
public:
	MGLInputItem(GLuint key, GLuint action1, GLuint action2, GLuint index, GLuint mod, void* funcData);
	~MGLInputItem() { functionData = nullptr; }

	void SetActive(GLboolean active) { inUse = active; }
	GLboolean KeyShouldRun() const { return inUse; }

	GLuint keyValue = 0;
	GLuint action1 = 0;
	GLuint action2 = 0;
	GLuint index = 0;
	GLuint keyMod = 0;

	GLboolean inUse = GL_FALSE;

	void* functionData = nullptr; // secodary data for this key function (blame GLFW)
};