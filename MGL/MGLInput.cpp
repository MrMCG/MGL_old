#include "stdafx.h"

#include "MGLInput.h"
#include "MGLWindow.h"
#include "MGLUtil.h"

MGLInput::MGLInput(MGLWindow* windo) : MGLInput() {
	AttatchInputToWindow(windo);
}

MGLInput::~MGLInput() {
	delete keyboadInput;
	delete mouseInput;
	window = nullptr;
}

void MGLInput::AttatchInputToWindow(MGLWindow* windo) {
	window = windo;
	glfwSetWindowUserPointer(window->GetGLFWWindow(), this);

	glfwSetKeyCallback(window->GetGLFWWindow(), reinterpret_cast<GLFWkeyfun>(this->KeyInputCallBack));
	glfwSetCursorEnterCallback(window->GetGLFWWindow(), reinterpret_cast<GLFWcursorenterfun>(this->MouseFocusCallBack));

	glfwSetMouseButtonCallback(window->GetGLFWWindow(), reinterpret_cast<GLFWmousebuttonfun>(this->MouseButtonCallBack));
	glfwSetCursorPosCallback(window->GetGLFWWindow(), reinterpret_cast<GLFWcursorposfun>(this->MousePositionCallBack));
	glfwSetScrollCallback(window->GetGLFWWindow(), reinterpret_cast<GLFWscrollfun>(this->MouseScrollCallBack));
}

void MGLInput::PollInput() const {
	keyboadInput->RunKeys();
	mouseInput->RunKeys();
}

void MGLInput::AddKeyboardFunction(const GLuint keyVal, const GLuint action, const MGLFunction2 callbackFunc, void* funcData, const GLuint mod) {
	keyboadInput->AddKeyFunction(keyVal, action, callbackFunc, funcData, mod);
}

void MGLInput::AddMouseFunction(const GLuint keyVal, const GLuint action, const MGLFunction2 callbackFunc, void* funcData, const GLuint mod) {
	mouseInput->AddKeyFunction(keyVal, action, callbackFunc, funcData, mod);
}

void MGLInput::AddScrollFunction(const MGLFunction2 func, void* funcData, const GLuint mod) {
	mouseInput->AddKeyFunction(MGLMouse::ScrollKey, MGLInputType::Scroll, func, funcData, mod);
}

void MGLInput::SetDataPointer(void* data) const {
	mouseInput->SetDataPointer(data);
	keyboadInput->SetDataPointer(data);
}

void MGLInput::KeyInputCallBack(GLFWwindow* window, GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	static auto game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleKeyInput(key, scancode, MGL::BitShiftLeftEnableZero(action), mods); // GLFW actions -> MGLInputType ButtonActions
}

void MGLInput::MouseButtonCallBack(GLFWwindow* window, GLuint button, GLuint action, GLuint mods) {
	static auto game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleMouseButton(button, MGL::BitShiftLeftEnableZero(action), mods); // GLFW actions -> MGLInputType ButtonActions
}

void MGLInput::MousePositionCallBack(GLFWwindow* window, GLdouble xPos, GLdouble yPos) {
	static auto game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleMousePosition(xPos, yPos);
}

void MGLInput::MouseScrollCallBack(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset) {
	static auto game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleMouseScroll(xOffset, yOffset);
}

void MGLInput::MouseFocusCallBack(GLFWwindow* window, GLboolean focused) {
	static auto game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleMouseFocus(focused);
}

void MGLInput::HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	keyboadInput->UpdateKey(key, mods, action);
}

void MGLInput::HandleMouseButton(GLuint button, GLuint action, GLuint mods) {
	mouseInput->UpdateKey(button, mods, action);
}

void MGLInput::HandleMousePosition(GLdouble xPos, GLdouble yPos) {
	if (window->GetInFocus()) {
		mouseInput->UpdatePosition(static_cast<GLfloat>(xPos), static_cast<GLfloat>(yPos));
	}
	
}

void MGLInput::HandleMouseScroll(GLdouble xOffset, GLdouble yOffset) {
	mouseInput->UpdateScroll(static_cast<GLfloat>(xOffset), static_cast<GLfloat>(yOffset));
	mouseInput->UpdateKey(MGLMouse::ScrollKey, 0, MGLInputType::Scroll);
}

void MGLInput::HandleMouseFocus(GLboolean focused) {
	window->SetInFocus(focused);
	glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, focused ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	
}
