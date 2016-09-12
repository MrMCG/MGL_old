#include "stdafx.h"

#include "MGLMouse.h"

void MGLMouse::RunKeys() {
	MGLInputType::RunKeys();
	ResetMouseOffset();
	ResetScrollOffset();
}

void MGLMouse::UpdatePosition(GLfloat xPos, GLfloat yPos) {
	mouseOffsetX = xPos - mouseLastX;
	mouseOffsetY = mouseLastY - yPos;
	mouseLastX = xPos;
	mouseLastY = yPos;
}

void MGLMouse::UpdateScroll(GLfloat x, GLfloat y) {
	scrollX = x; scrollY = y;
}

void MGLMouse::ResetMouseOffset() {
	mouseOffsetX = 0;
	mouseOffsetY = 0;
}

void MGLMouse::ResetScrollOffset() {
	scrollX = 0;
	scrollY = 0;
}

