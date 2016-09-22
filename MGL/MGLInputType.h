#pragma once
#include "stdafx.h"

#include "MGLInputItem.h"
#include "MGLUtil.h"

/*
*  TODO: refactor dataPointer memory control
*/

class MGLInputType {
public:
	MGLInputType() {}
	virtual ~MGLInputType();

	enum ButtonActions { Released = 1, Pressed = 2, Repeat = 4, Scroll = 8 };

	virtual void AddKeyFunction(const GLuint keyVal, const GLuint action, const MGLFunction2 callbackFunc, void* funcData, const GLuint mod = 0);

	virtual void UpdateKey(GLuint keyVal, GLuint mod, GLuint action);
	virtual void RunKeys();

	void SetDataPointer(void* pointer) { dataPointer = pointer; }

	MGLInputType(const MGLInputType& other) = delete;
	MGLInputType(const MGLInputType&& other) = delete;
	MGLInputType& operator=(const MGLInputType& other) = delete;
	MGLInputType& operator=(const MGLInputType&& other) = delete;

protected:

	void RunSingleKey(const MGLInputItem& key) const;
	void UpdateRegisteredKey(MGLInputItem& key) const; 
	GLboolean FindRegisteredKey(const MGLInputItem& key) const;

	void SetCurrentData(const GLuint keyVal, const GLuint mod, const GLuint action);

	GLuint inputCounter = 0;
	void* dataPointer = nullptr; // primary data for all key functions (blame GLFW)

	std::vector<MGLInputItem>* activeKeysVector = new std::vector<MGLInputItem>();
	std::unordered_map<GLuint, MGLFunction2>* keyFunctionsMap = new std::unordered_map<GLuint, MGLFunction2>();

	GLuint currentMod = 0;
	GLuint currentKey = 0;
	GLuint currentAction = 0;
};
