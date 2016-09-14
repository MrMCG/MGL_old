#pragma once
#include "stdafx.h"

#include "MGLMouse.h"
#include "MGLKeyboard.h"

/*
 * TODO: refactor adding keys into a more managable solution (its currently too... messy)
 * TODO: add the ablility to "swap out" inputs to register different key entries
 */

class MGLWindow;

class MGLInput {
public:
	MGLInput() {}
	explicit MGLInput(MGLWindow* windo);
	virtual ~MGLInput() {}

	void AttatchInputToWindow(MGLWindow* windo);
	void PollInput() const;

	virtual void AddKeyboardFunction(GLuint keyVal, GLuint action, MGLFunction2 callbackFunc, void* funcData, GLuint mod = 0);
	virtual void AddKeyboardFunction(GLuint keyVal, GLuint firstAction, GLuint secondAction, MGLFunction2 callbackFunc, void* funcData, GLuint mod = 0);
	virtual void AddMouseFunction(GLuint keyVal, GLuint action, MGLFunction2 callbackFunc, void* funcData, GLuint mod = 0);
	virtual void AddMouseFunction(GLuint keyVal, GLuint firstAction, GLuint secondAction, MGLFunction2 callbackFunc, void* funcData, GLuint mod = 0);
	virtual void AddScrollFunction(MGLFunction2 func, void* funcData, GLuint mod = 0);
	void SetDataPointer(void* data) const;

	GLfloat GetMouseOffsetX() const { return mouseInput->GetMouseOffsetX(); }
	GLfloat GetMouseOffsetY() const { return mouseInput->GetMouseOffsetY(); }
	GLfloat GetScrollX() const { return mouseInput->GetScrollX(); }
	GLfloat GetScrollY() const { return mouseInput->GetScrollY(); }

	MGLInput(const MGLInput& other) = delete; 
	MGLInput(const MGLInput&& other) = delete; 
	MGLInput& operator=(const MGLInput& other) = delete; 
	MGLInput& operator=(const MGLInput&& other) = delete; 

private:

	static void KeyInputCallBack(GLFWwindow* window, GLuint key, GLuint scancode, GLuint action, GLuint mods);
	static void MouseButtonCallBack(GLFWwindow* window, GLuint button, GLuint action, GLuint mods);
	static void MousePositionCallBack(GLFWwindow* window, GLdouble xPos, GLdouble yPos);
	static void MouseScrollCallBack(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset);
	static void MouseFocusCallBack(GLFWwindow* window, GLboolean focused);

protected:

	virtual void HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods);
	virtual void HandleMouseButton(GLuint button, GLuint action, GLuint mods);
	virtual void HandleMousePosition(GLdouble xPos, GLdouble yPos);
	virtual void HandleMouseScroll(GLdouble xOffset, GLdouble yOffset);
	virtual void HandleMouseFocus(GLboolean focused);


	MGLKeyboard* keyboadInput = new MGLKeyboard();
	MGLMouse* mouseInput = new MGLMouse();
	MGLWindow* window = nullptr;

};