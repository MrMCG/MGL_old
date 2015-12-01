#pragma once

#include "glew.h"
#include "wglew.h"
#include "SOIL.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SOIL.lib")

class Test {
public:
	static int add(int a, int b) { return a + b; glEnable(GL_DEPTH_TEST); };
};