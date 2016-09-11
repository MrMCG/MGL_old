#pragma once
#include "stdafx.h"

#include "MGLInputType.h"

#define MGL_INPUT_SCROLLKEY 40
#define MGL_INPUT_SCROLLACTION 5

class MGLMouse : public MGLInputType {
public:
	MGLMouse() {}
	~MGLMouse() {}

	virtual void UpdatePosition(GLfloat xPos, GLfloat yPos);
	virtual void UpdateScroll(GLfloat x, GLfloat y);

	virtual void RunKeys() override;

	GLfloat GetMouseOffsetX() const { return mouseOffsetX; }
	GLfloat GetMouseOffsetY() const { return mouseOffsetY; }
	GLfloat GetScrollX() const { return scrollX; }
	GLfloat GetScrollY() const { return scrollY; }

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