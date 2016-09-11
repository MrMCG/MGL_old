#pragma once
#include "stdafx.h"

// Key function data
class MGLInputItem {
public:
	MGLInputItem(GLuint k, GLuint a1, GLuint a2, GLuint i, GLuint m, void* funcData);

	GLboolean InUse() { return inUse; }
	GLboolean KeyShouldRun();

	GLuint keyValue;
	GLuint action1;
	GLuint action2;
	GLuint index;
	GLuint keyMod;

	GLboolean inUse = GL_FALSE;

	void* functionData = nullptr; // secodary data for this key function (blame GLFW)
};