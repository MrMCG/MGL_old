#include "stdafx.h"

#include "MGLInput.h"

MGLInput::MGLInput() {
	keyboadInput = std::make_unique<MGLKeyboard>();
	mouseInput = std::make_unique<MGLMouse>();
}

void MGLInput::AttatchInputToWindow(MGLWindow* window) {

}