#include "stdafx.h"
#include "MGLInputType.h"

void MGLInputType::AddKeyFunction(GLuint key, GLuint action, MGLFunction2 func, void* funcData, GLuint mod) {
	AddKeyFunction(key, action, action, func, funcData, mod);
}

void MGLInputType::AddKeyFunction(GLuint key, GLuint firstAction, GLuint secondAction, MGLFunction2 func, void* funcData, GLuint mod) {
	GLuint index = ++inputCounter; // increase index count
	inputVector.push_back(MGLInputItem(key, firstAction, secondAction, index, mod, funcData)); // add function
	inputMap.insert(std::make_pair(index, func)); // map index to function
}

void MGLInputType::UpdateKey(GLuint key, GLuint action) {
	for (MGLInputItem& obj : inputVector) {
		if (obj.keyValue == key) { // key has a function
			if (obj.action1 == action || obj.action2 == action) { // correct action
				obj.inUse = GL_TRUE;
			}
			else
				obj.inUse = GL_FALSE;
		}
	}
}

void MGLInputType::RunKeys() {
	for (MGLInputItem& obj : inputVector) {
		if (obj.inUse) { // keys in use
			if (!obj.keyMod) // doesnt require mod
				(inputMap[obj.index])(dataPointer, obj.functionData);
			else if (obj.keyMod == currentMod) // requires mod
				(inputMap[obj.index])(dataPointer, obj.functionData);
		}
	}
}

void MGLInputType::RunSingleKey(MGLInputItem& key) {

}
