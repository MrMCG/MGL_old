#pragma once
#include "stdafx.h"

#include "MGLInputItem.h"
#include "MGLUtil.h"

class MGLInputType {
public:
	MGLInputType();
	virtual ~MGLInputType() {};

	virtual void AddKeyFunction(GLuint keyVal,	GLuint action, MGLFunction2 callbackFunc, void* funcData, GLuint mod = 0);
	virtual void AddKeyFunction(GLuint keyVal,	GLuint firstAction, GLuint secondAction, MGLFunction2 callbackFunc, void* funcData, GLuint mod = 0);

	virtual void UpdateKey(GLuint keyVal, GLuint mod, GLuint action);
	virtual void RunKeys();

	void SetDataPointer(void* pointer) { dataPointer = pointer; }

protected:

	void RunSingleKey(const MGLInputItem& key) const;
	void UpdateRegisteredKey(MGLInputItem& key) const; 
	GLboolean FindRegisteredKey(const MGLInputItem& key) const;

	void SetCurrentData(GLuint keyVal, GLuint mod, GLuint action);

	GLuint inputCounter = 0;
	void* dataPointer = nullptr; // primary data for all key functions

	std::unique_ptr<std::vector<MGLInputItem>> activeKeysVector;
	std::unique_ptr<std::unordered_map<GLuint, MGLFunction2>> keyFunctionsMap;

	GLuint currentMod = 0;
	GLuint currentKey = 0;
	GLuint currentAction = 0;
};
