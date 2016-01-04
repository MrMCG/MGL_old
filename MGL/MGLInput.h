#include "stdafx.h"
#pragma once

#include <unordered_map>

struct MGLInputItem {
	MGLInputItem(GLuint k, GLuint a1, GLuint a2, GLuint i, GLuint m) {
		m_key = k; m_action1 = a1; 
		m_action2 = a2; m_index = i;
		m_mod = m;
	}
	GLuint m_key;
	GLuint m_action1;
	GLuint m_action2;
	GLuint m_index;
	GLuint m_mod;
	GLboolean m_use = GL_FALSE;
};

class MGLInput {
public:
	MGLInput() : m_dataPointer(nullptr), m_inputCounter(0), m_currentMod(0) {}
	~MGLInput(){ m_dataPointer = nullptr; }

	virtual void AddKeyFunction(GLuint key, GLuint action, MGLFunction func, GLuint mod = 0);
	virtual void AddKeyFunction(GLuint key, GLuint firstAction, GLuint secondAction, MGLFunction func, GLuint mod = 0);

	virtual void UpdateKey(GLuint Key, GLuint action);
	inline void UpdateMod(GLuint mod) { m_currentMod = mod; }
	virtual void RunKeys();

	inline void SetDataPointer(void* pointer) { m_dataPointer = pointer; }

protected:
	GLuint m_inputCounter;
	GLuint m_currentMod;
	void* m_dataPointer;

	std::vector<MGLInputItem> m_inputList;
	std::unordered_map<GLuint, MGLFunction> m_inputMap;
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

	virtual void UpdatePosition(GLfloat xPos, GLfloat yPos);

	inline virtual void UpdateScroll(GLfloat x, GLfloat y) 
		{ m_scrollX = x; m_scrollY = y; m_scrollUpdated = GL_TRUE; }

	virtual void RunKeys() override;
	virtual void AddScrollFunction(MGLFunction func, GLuint mod = 0);

	GLfloat GetOffsetX() const { return m_offsetX; }
	GLfloat GetOffsetY() const { return m_offsetY; }
	GLfloat GetScrollX() const { return m_scrollX; }
	GLfloat GetScrollY() const { return m_scrollY; }

	GLboolean GetHasUpdated() const { return m_hasUpdated; }
	void SetHasUpdated(GLboolean updated) { m_hasUpdated = updated; }
	void SetScrollUpdated(GLboolean updated) { m_scrollUpdated = updated; }

protected:
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

