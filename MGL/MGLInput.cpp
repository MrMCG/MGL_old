#include "stdafx.h"
#include "MGLInput.h"

/********************************/
/*********** MGLInput ***********/
/********************************/

void MGLInput::AddKeyFunction(GLuint key, GLuint action, MGLFunction func, GLuint mod) {
	AddKeyFunction(key, action, action, func, mod);
}

void MGLInput::AddKeyFunction(GLuint key, GLuint firstAction, GLuint secondAction, MGLFunction func, GLuint mod) {
	GLuint index = ++m_inputCounter;
	m_inputList.push_back(MGLInputItem(key, firstAction, secondAction, index, mod));
	m_inputMap.insert(std::make_pair(index, func));
}

void MGLInput::UpdateKey(GLuint key, GLuint action) {
	for (MGLInputItem& obj : m_inputList) {
		if (obj.m_key == key) {
			if (obj.m_action1 == action || obj.m_action2 == action) {
				obj.m_use = GL_TRUE;
			}
			else
				obj.m_use = GL_FALSE;
		}
	}
}

void MGLInput::RunKeys() {
	for (MGLInputItem& obj : m_inputList) {
		if (obj.m_use) {
			if (!obj.m_mod)
				(this->m_inputMap[obj.m_index])(m_dataPointer);
			else if (obj.m_mod == m_currentMod)
				(this->m_inputMap[obj.m_index])(m_dataPointer);
		}
	}
}

/********************************/
/*********** MGLMouse ***********/
/********************************/

MGLMouse::MGLMouse() {
	m_lastX = 0; 
	m_lastY = 0;
	m_offsetX = 0;
	m_offsetY = 0;

	m_scrollX = 0;
	m_scrollY = 0;
	m_hasUpdated = GL_FALSE;

	m_scrollKey = 40;
}

void MGLMouse::RunKeys() {
	for (MGLInputItem& obj : m_inputList) {
		if (obj.m_use) {
			if (!obj.m_mod)
				(this->m_inputMap[obj.m_index])(m_dataPointer);
			else if (obj.m_mod == m_currentMod)
				(this->m_inputMap[obj.m_index])(m_dataPointer);
		} else if (obj.m_key == m_scrollKey && obj.m_action1 == 5 && m_scrollUpdated)
			(this->m_inputMap[obj.m_index])(m_dataPointer);
	}
}

void MGLMouse::AddScrollFunction(MGLFunction func, GLuint mod) {
	AddKeyFunction(m_scrollKey, 5, func, mod);
}

void MGLMouse::UpdatePosition(GLfloat xPos, GLfloat yPos) {
	m_offsetX = xPos - m_lastX;
	m_offsetY = m_lastY - yPos;
	m_lastX = xPos;
	m_lastY = yPos;
	m_hasUpdated = GL_TRUE;
}

