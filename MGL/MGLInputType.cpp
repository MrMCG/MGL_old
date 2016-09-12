#include "stdafx.h"
#include "MGLInputType.h"

MGLInputType::MGLInputType() {
	activeKeysVector = std::make_unique<std::vector<MGLInputItem>>();
	keyFunctionsMap = std::make_unique<std::unordered_map<GLuint, MGLFunction2>>();
	dataPointer = nullptr;
}

void MGLInputType::AddKeyFunction(GLuint keyVal, GLuint action, MGLFunction2 callbackFunc, void* funcData, GLuint mod) {
	AddKeyFunction(keyVal, action, action, callbackFunc, funcData, mod);
}

void MGLInputType::AddKeyFunction(GLuint keyVal, GLuint firstAction, GLuint secondAction, MGLFunction2 callbackFunc, void* funcData, GLuint mod) {
	++inputCounter;
	activeKeysVector->push_back(MGLInputItem(keyVal, firstAction, secondAction, inputCounter, mod, funcData));
	keyFunctionsMap->insert(std::make_pair(inputCounter, callbackFunc));
}

void MGLInputType::UpdateKey(GLuint keyVal, GLuint mod, GLuint action) {
	SetCurrentData(keyVal, mod, action);

	for (auto& key : *activeKeysVector) {
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

GLboolean MGLInputType::FindRegisteredKey(const MGLInputItem& key) const {
	return key.keyValue == currentKey ? GL_TRUE : GL_FALSE;
}

void MGLInputType::UpdateRegisteredKey(MGLInputItem& key) const
{
	if (key.action1 == currentAction || key.action2 == currentAction) 
		key.SetActive(GL_TRUE);
	else
		key.SetActive(GL_FALSE);
}

void MGLInputType::RunKeys() {
	for (auto& key : *activeKeysVector) {
		if (key.KeyShouldRun()) { 
			RunSingleKey(key);		
		}
	}
}

void MGLInputType::RunSingleKey(const MGLInputItem& key) const {
	keyFunctionsMap->at(key.index)(dataPointer, key.functionData);
}
