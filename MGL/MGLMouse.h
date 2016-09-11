#pragma once
#include "stdafx.h"

#include "MGLInputType.h"

class MGLMouse : public MGLInputType {
public:
	MGLMouse() {}
	~MGLMouse() {}

	virtual void UpdatePosition(GLfloat xPos, GLfloat yPos);
	virtual void UpdateScroll(GLfloat x, GLfloat y)	{
		scrollX = x; scrollY = y; scrollUpdated = GL_TRUE;
	}

	virtual void RunKeys() override;
	virtual void AddScrollFunction(MGLFunction2 func, void* funcData, GLuint mod = 0);

	GLfloat GetOffsetX() const { return offsetX; }
	GLfloat GetOffsetY() const { return offsetY; }
	GLfloat GetScrollX() const { return scrollX; }
	GLfloat GetScrollY() const { return scrollY; }

	GLboolean GetHasUpdated() const { return hasUpdated; }

	void SetHasUpdated(GLboolean updated) { hasUpdated = updated; }
	void SetScrollUpdated(GLboolean updated) { scrollUpdated = updated; }

protected:

	GLfloat lastX = 0;
	GLfloat lastY = 0;
	GLfloat offsetX = 0;
	GLfloat offsetY = 0;

	GLuint scrollKey = 40;
	GLfloat scrollX = 0;
	GLfloat scrollY = 0;

	GLboolean hasUpdated = GL_FALSE;
	GLboolean scrollUpdated = GL_FALSE;
};