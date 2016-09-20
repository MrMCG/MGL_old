#pragma once
#include "stdafx.h"

/*
 *  TODO: refactor functionData memory control
 */

class MGLInputItem {
public:
	MGLInputItem(const GLuint key, const GLuint action1, const GLuint action2, const GLuint index, const GLuint mod, void* funcData);
	~MGLInputItem() { functionData = nullptr; }

	void SetActive(const GLboolean active) { inUse = active; }
	GLboolean KeyShouldRun() const { return inUse; }

	GLuint keyValue = 0;
	GLuint action1 = 0;
	GLuint action2 = 0;
	GLuint index = 0;
	GLuint keyMod = 0;

	GLboolean inUse = GL_FALSE;

	void* functionData = nullptr; // secodary data for this key function (blame GLFW)

	MGLInputItem(const MGLInputItem&& other) noexcept;
	
	MGLInputItem(const MGLInputItem& other) = delete;
	MGLInputItem& operator=(const MGLInputItem& other) = delete;
	MGLInputItem& operator=(const MGLInputItem&& other) = delete;

};