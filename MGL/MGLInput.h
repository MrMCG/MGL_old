#pragma once
#include "stdafx.h"

#include "MGLMouse.h"
#include "MGLKeyboard.h"

class MGLWindow;

class MGLInput {
public:
	MGLInput();
	~MGLInput() {}

	void AttatchInputToWindow(MGLWindow* window);

protected:
	std::unique_ptr<MGLKeyboard> keyboadInput;
	std::unique_ptr<MGLMouse> mouseInput;
};