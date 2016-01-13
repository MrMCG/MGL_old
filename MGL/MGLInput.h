#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

// Key function data
struct MGLInputItem {
	MGLInputItem(GLuint k, GLuint a1, GLuint a2, GLuint i, GLuint m, void* funcData) {
		m_key = k; m_action1 = a1; 
		m_action2 = a2; m_index = i;
		m_mod = m; m_functionData = funcData;
	}
	GLuint m_key;
	GLuint m_action1;
	GLuint m_action2;
	GLuint m_index;
	GLuint m_mod;
	GLboolean m_use = GL_FALSE;

	void* m_functionData = nullptr; // secodary data for this key function
};

class MGLInput {
public:
	MGLInput() : m_dataPointer(nullptr), m_inputCounter(0), m_currentMod(0) {}
	~MGLInput(){ m_dataPointer = nullptr; }

	// Adds function to key with only one action
	virtual void AddKeyFunction(GLuint key,
		GLuint action, MGLFunction2 func, void* funcData, GLuint mod = 0);
	// Adds function to key with two actions
	virtual void AddKeyFunction(GLuint key,
		GLuint firstAction, GLuint secondAction, MGLFunction2 func, void* funcData, GLuint mod = 0);
	// Updates the state of current keys to run
	virtual void UpdateKey(GLuint Key, GLuint action);
	// Updates the state of the current active mods
	void UpdateMod(GLuint mod) { m_currentMod = mod; }
	// Runs active keys with mods
	virtual void RunKeys();
	// Sets user data pointer for data access in function
	void SetDataPointer(void* pointer) { m_dataPointer = pointer; }

protected:
	GLuint m_inputCounter;
	GLuint m_currentMod;
	void* m_dataPointer; // primary data for all key functions

	std::vector<MGLInputItem> m_inputList;
	std::unordered_map<GLuint, MGLFunction2> m_inputMap;
};

class MGLKeyboard : public MGLInput{
public: 
	MGLKeyboard(){}
	~MGLKeyboard(){}
};

class MGLMouse : public MGLInput {
public:
	MGLMouse();
	~MGLMouse(){}
	// Update current mouse position
	virtual void UpdatePosition(GLfloat xPos, GLfloat yPos);
	// Update scroll state
	virtual void UpdateScroll(GLfloat x, GLfloat y) 
		{ m_scrollX = x; m_scrollY = y; m_scrollUpdated = GL_TRUE; }
	// Run mouse functions based on current state
	virtual void RunKeys() override;
	// Adds a scroll function (user handles scroll up (+1) and scroll down (-1))
	virtual void AddScrollFunction(MGLFunction2 func, void* funcData, GLuint mod = 0);

	// Amount of change since last known mouse position, X axis
	GLfloat GetOffsetX() const { return m_offsetX; }
	// Amount of change since last known mouse position, Y axis
	GLfloat GetOffsetY() const { return m_offsetY; }
	// Amount scrolled left/right (+1/-1)
	GLfloat GetScrollX() const { return m_scrollX; }
	// Amount scrolled up/down (+1/-1)
	GLfloat GetScrollY() const { return m_scrollY; }

	// Has the mouse recently been updated (moved)
	GLboolean GetHasUpdated() const { return m_hasUpdated; }
	
	void SetHasUpdated(GLboolean updated) { m_hasUpdated = updated; }
	void SetScrollUpdated(GLboolean updated) { m_scrollUpdated = updated; }

protected:

	/****** Data ******/

	GLfloat m_lastX;
	GLfloat m_lastY;
	GLfloat m_offsetX;
	GLfloat m_offsetY;

	GLuint m_scrollKey;
	GLfloat m_scrollX;
	GLfloat m_scrollY;

	GLboolean m_hasUpdated;
	GLboolean m_scrollUpdated;
};

