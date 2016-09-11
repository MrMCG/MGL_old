#pragma once
#include "stdafx.h"

#include "MGLMouse.h"

void MGLMouse::RunKeys() {
	for (MGLInputItem& obj : inputVector) {
		if (obj.inUse) {
			if (!obj.keyMod)
				(inputMap[obj.index])(dataPointer, obj.functionData);
			else if (obj.keyMod == currentMod)
				(inputMap[obj.index])(dataPointer, obj.functionData);
		}
		else if (obj.keyValue == scrollKey && obj.action1 == MGL_INPUT_SCROLLACTION && scrollUpdated)
			(inputMap[obj.index])(dataPointer, obj.functionData);
	}
}

void MGLMouse::AddScrollFunction(MGLFunction2 func, void* funcData, GLuint mod) {
	AddKeyFunction(scrollKey, MGL_INPUT_SCROLLACTION, func, funcData, mod);
}

void MGLMouse::UpdatePosition(GLfloat xPos, GLfloat yPos) {
	offsetX = xPos - lastX;
	offsetY = lastY - yPos;
	lastX = xPos;
	lastY = yPos;
	hasUpdated = GL_TRUE;
}