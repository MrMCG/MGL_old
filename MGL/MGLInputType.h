#pragma once
#include "stdafx.h"

#include "MGLInputItem.h"
#include "MGLUtil.h"

class MGLInputType {
public:
	MGLInputType();
	~MGLInputType();

	virtual void AddKeyFunction(GLuint keyVal,	GLuint action, MGLFunction2 callbackFunc, void* funcData, GLuint mod = 0);
	virtual void AddKeyFunction(GLuint keyVal,	GLuint firstAction, GLuint secondAction, MGLFunction2 callbackFunc, void* funcData, GLuint mod = 0);

	virtual void UpdateKey(GLuint keyVal, GLuint mod, GLuint action);
	virtual void RunKeys();

	void SetDataPointer(void* pointer) { dataPointer = pointer; }

protected:

	void RunSingleKey(MGLInputItem& key);
	void UpdateRegisteredKey(MGLInputItem& key);
	GLboolean FindRegisteredKey(MGLInputItem& key);

	void SetCurrentData(GLuint keyVal, GLuint mod, GLuint action);

	GLuint inputCounter = 0;
	void* dataPointer = nullptr; // primary data for all key functions

	std::unique_ptr<std::vector<MGLInputItem>> activeKeysVector;
	std::unique_ptr<std::unordered_map<GLuint, MGLFunction2>> keyFunctionsMap;

	GLuint currentMod = -1;
	GLuint currentKey = -1;
	GLuint currentAction = -1;
};