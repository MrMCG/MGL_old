#pragma once
#include "stdafx.h"

#include "MGLMouse.h"
#include "MGLKeyboard.h"

class MGLWindow;

class MGLInput {
public:
	MGLInput();
	explicit MGLInput(const std::shared_ptr<MGLWindow> windo);
	virtual ~MGLInput() {}

	void AttatchInputToWindow(const std::shared_ptr<MGLWindow> windo);
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

	MGLInput(const MGLInput& other) = delete; // copy constructor
	MGLInput(const MGLInput&& other) = delete; // move constructor
	MGLInput& operator=(const MGLInput& other) = delete; // copy assignment
	MGLInput& operator=(const MGLInput&& other) = delete; // move assignment

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


	std::unique_ptr<MGLKeyboard> keyboadInput;
	std::unique_ptr<MGLMouse> mouseInput;
	std::weak_ptr<MGLWindow> window;

};