#include "stdafx.h"

#include "MGLInput.h"
#include "MGLWindow.h"

MGLInput::MGLInput() {
	keyboadInput = std::make_unique<MGLKeyboard>();
	mouseInput = std::make_unique<MGLMouse>();
}

void MGLInput::AttatchInputToWindow(const std::shared_ptr<MGLWindow> windo) {
	window = windo;
	if (auto win = window.lock()) {
		glfwSetWindowUserPointer(win->GetGLFWWindow(), this);

		glfwSetKeyCallback(win->GetGLFWWindow(), (GLFWkeyfun)this->KeyInputCallBack);
		glfwSetCursorEnterCallback(win->GetGLFWWindow(), (GLFWcursorenterfun)this->MouseFocusCallBack);

		glfwSetMouseButtonCallback(win->GetGLFWWindow(), (GLFWmousebuttonfun)this->MouseButtonCallBack);
		glfwSetCursorPosCallback(win->GetGLFWWindow(), (GLFWcursorposfun)this->MousePositionCallBack);
		glfwSetScrollCallback(win->GetGLFWWindow(), (GLFWscrollfun)this->MouseScrollCallBack);
	}
}

void MGLInput::PollInput() {
	keyboadInput->RunKeys();
	mouseInput->RunKeys();
}

void MGLInput::AddKeyboardFunction(GLuint keyVal, GLuint action, MGLFunction2 callbackFunc, void* funcData, GLuint mod) {
	keyboadInput->AddKeyFunction(keyVal, action, callbackFunc, funcData, mod);
}

void MGLInput::AddKeyboardFunction(GLuint keyVal, GLuint firstAction, GLuint secondAction, MGLFunction2 callbackFunc, void* funcData, GLuint mod) {
	keyboadInput->AddKeyFunction(keyVal, firstAction, secondAction, callbackFunc, funcData, mod);
}

void MGLInput::AddMouseFunction(GLuint keyVal, GLuint action, MGLFunction2 callbackFunc, void* funcData, GLuint mod) {
	mouseInput->AddKeyFunction(keyVal, action, callbackFunc, funcData, mod);
}

void MGLInput::AddMouseFunction(GLuint keyVal, GLuint firstAction, GLuint secondAction, MGLFunction2 callbackFunc, void* funcData, GLuint mod) {
	mouseInput->AddKeyFunction(keyVal, firstAction, secondAction, callbackFunc, funcData, mod);
}

void MGLInput::AddScrollFunction(MGLFunction2 func, void* funcData, GLuint mod) {
	mouseInput->AddKeyFunction(MGL_INPUT_SCROLLKEY, MGL_INPUT_SCROLLACTION, func, funcData, mod);
}

void MGLInput::SetDataPointer(void* data) {
	mouseInput->SetDataPointer(data);
	keyboadInput->SetDataPointer(data);
}

void MGLInput::KeyInputCallBack(GLFWwindow* window, GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	MGLInput* game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleKeyInput(key, scancode, action, mods);
}

void MGLInput::MouseButtonCallBack(GLFWwindow* window, GLuint button, GLuint action, GLuint mods) {
	MGLInput* game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleMouseButton(button, action, mods);
}

void MGLInput::MousePositionCallBack(GLFWwindow* window, GLdouble xPos, GLdouble yPos) {
	MGLInput* game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleMousePosition(xPos, yPos);
}

void MGLInput::MouseScrollCallBack(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset) {
	MGLInput* game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleMouseScroll(xOffset, yOffset);
}

void MGLInput::MouseFocusCallBack(GLFWwindow* window, GLboolean focused) {
	MGLInput* game = static_cast<MGLInput*>(glfwGetWindowUserPointer(window));
	game->HandleMouseFocus(focused);
}

void MGLInput::HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) {
	keyboadInput->UpdateKey(key, mods, action);
}

void MGLInput::HandleMouseButton(GLuint button, GLuint action, GLuint mods) {
	mouseInput->UpdateKey(button, mods, action);
}

void MGLInput::HandleMousePosition(GLdouble xPos, GLdouble yPos) {
	if (auto win = window.lock()) {
		if (win->GetInFocus()) {
			mouseInput->UpdatePosition((GLfloat)xPos, (GLfloat)yPos);
		}
	}
}

void MGLInput::HandleMouseScroll(GLdouble xOffset, GLdouble yOffset) {
	mouseInput->UpdateScroll((GLfloat)xOffset, (GLfloat)yOffset);
	mouseInput->UpdateKey(MGL_INPUT_SCROLLKEY, 0, MGL_INPUT_SCROLLACTION);
}

void MGLInput::HandleMouseFocus(GLboolean focused) {
	if (auto win = window.lock()) {
		win->SetInFocus(focused);
		glfwSetInputMode(win->GetGLFWWindow(), GLFW_CURSOR, focused ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}
}


