#pragma once
#include "stdafx.h"

#include "MGLInputItem.h"
#include "MGLUtil.h"

class MGLInputType {
public:
	MGLInputType() : dataPointer(nullptr), inputCounter(0), currentMod(0) {}
	~MGLInputType() { dataPointer = nullptr; }

	virtual void AddKeyFunction(GLuint key,
		GLuint action, MGLFunction2 func, void* funcData, GLuint mod = 0);
	virtual void AddKeyFunction(GLuint key,
		GLuint firstAction, GLuint secondAction, MGLFunction2 func, void* funcData, GLuint mod = 0);

	virtual void UpdateKey(GLuint Key, GLuint action);
	virtual void RunKeys();
	void UpdateMod(GLuint mod) { currentMod = mod; }
	void SetDataPointer(void* pointer) { dataPointer = pointer; }

protected:

	void RunSingleKey(MGLInputItem& key);


	GLuint inputCounter;
	GLuint currentMod;
	void* dataPointer; // primary data for all key functions

	std::vector<MGLInputItem> inputVector;
	std::unordered_map<GLuint, MGLFunction2> inputMap;
};