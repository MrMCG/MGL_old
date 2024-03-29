#pragma once
#include "stdafx.h"

#include "MGLInputType.h"

class MGLMouse : public MGLInputType {
public:
	MGLMouse() {}

	virtual void UpdatePosition(const GLfloat xPos, const GLfloat yPos);
	virtual void UpdateScroll(const GLfloat x, const GLfloat y);

	void RunKeys() override;

	GLfloat GetMouseOffsetX() const { return mouseOffsetX; }
	GLfloat GetMouseOffsetY() const { return mouseOffsetY; }
	GLfloat GetScrollX() const { return scrollX; }
	GLfloat GetScrollY() const { return scrollY; }

	static const GLuint ScrollKey = 40;

protected:

	void ResetMouseOffset();
	void ResetScrollOffset();

	GLfloat mouseLastX = 0;
	GLfloat mouseLastY = 0;
	GLfloat mouseOffsetX = 0;
	GLfloat mouseOffsetY = 0;

	GLfloat scrollX = 0;
	GLfloat scrollY = 0;
};