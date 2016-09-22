#pragma once
#include "stdafx.h"

/*
 *  TODO: refactor functionData memory control
 */

class MGLInputItem {
public:
	MGLInputItem(const GLuint key, const GLuint actn, const GLuint index, const GLuint mod, void* funcData);
	~MGLInputItem();

	void SetActive(const GLboolean active) { inUse = active; }
	void SetDeleteFuncData(const GLboolean delData) { deleteFuncData = delData; }
	GLboolean KeyShouldRun() const { return inUse; }

	GLuint keyValue = 0;
	GLuint action = 0;
	GLuint index = 0;
	GLuint keyMod = 0;

	GLboolean inUse = GL_FALSE;
	GLboolean deleteFuncData = GL_TRUE;

	void* functionData = nullptr; // secodary data for this key function (blame GLFW)

	MGLInputItem(const MGLInputItem&& other) noexcept;
	
	MGLInputItem(const MGLInputItem& other) = delete;
	MGLInputItem& operator=(const MGLInputItem& other) = delete;
	MGLInputItem& operator=(const MGLInputItem&& other) = delete;

};