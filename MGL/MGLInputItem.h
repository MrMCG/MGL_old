#pragma once
#include "stdafx.h"

class MGLInputItem {
public:
	MGLInputItem(GLuint k, GLuint a1, GLuint a2, GLuint i, GLuint m, void* funcData);

	void SetActive(GLboolean active) { inUse = active; }
	GLboolean KeyShouldRun() { return inUse; }

	GLuint keyValue = -1;
	GLuint action1 = -1;
	GLuint action2 = -1;
	GLuint index = -1;
	GLuint keyMod = -1;

	GLboolean inUse = GL_FALSE;

	void* functionData = nullptr; // secodary data for this key function (blame GLFW)
};