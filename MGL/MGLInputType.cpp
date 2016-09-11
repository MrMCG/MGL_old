#include "stdafx.h"
#include "MGLInputType.h"

MGLInputType::MGLInputType() {
	activeKeysVector = std::make_unique<std::vector<MGLInputItem>>();
	keyFunctionsMap = std::make_unique<std::unordered_map<GLuint, MGLFunction2>>();
	dataPointer = nullptr;
}

MGLInputType::~MGLInputType() {
}

void MGLInputType::AddKeyFunction(GLuint keyVal, GLuint action, MGLFunction2 callbackFunc, void* funcData, GLuint mod) {
	AddKeyFunction(keyVal, action, action, callbackFunc, funcData, mod);
}

void MGLInputType::AddKeyFunction(GLuint keyVal, GLuint firstAction, GLuint secondAction, MGLFunction2 callbackFunc, void* funcData, GLuint mod) {
	GLuint index = ++inputCounter;
	activeKeysVector->push_back(MGLInputItem(keyVal, firstAction, secondAction, index, mod, funcData)); 
	keyFunctionsMap->insert(std::make_pair(index, callbackFunc));
}

void MGLInputType::UpdateKey(GLuint keyVal, GLuint mod, GLuint action) {
	SetCurrentData(keyVal, mod, action);

	for (MGLInputItem& key : *activeKeysVector) {
		if (FindRegisteredKey(key)) {
			UpdateRegisteredKey(key);
			break;
		}
	}
}

void MGLInputType::SetCurrentData(GLuint keyVal, GLuint mod, GLuint action) {
	currentMod = mod;
	currentKey = keyVal;
	currentAction = action;
}

GLboolean MGLInputType::FindRegisteredKey(MGLInputItem& key) {
	return key.keyValue == currentKey ? GL_TRUE : GL_FALSE;
}

void MGLInputType::UpdateRegisteredKey(MGLInputItem& key) {
	if (key.action1 == currentAction || key.action2 == currentAction) 
		key.SetActive(GL_TRUE);
	else
		key.SetActive(GL_FALSE);
}

void MGLInputType::RunKeys() {
	for (MGLInputItem& key : *activeKeysVector) {
		if (key.KeyShouldRun()) { 
			RunSingleKey(key);		
		}
	}
}

void MGLInputType::RunSingleKey(MGLInputItem& key) {
	keyFunctionsMap->at(key.index)(dataPointer, key.functionData);
}
