// Pre-Compiled headers
#pragma once

/*
	NOTE: MGLDEBUG is defined in VS properties for debug build
*/

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

/****** FOR MEMORY DEBUGGING *****/
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

// STL

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <fstream>

#include <algorithm>
#include <functional>
#include <ctime>
#include <mutex>
#include <memory>

#include <sstream>
#include <algorithm>
#include <thread>

#include <windows.h>

// Third Party

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "stb_image.h"

#pragma comment(lib, "opengl32.lib")

#ifdef MGLDEBUG
#pragma comment(lib, "libglew32d.lib")
#else
#pragma comment(lib, "libglew32.lib")
#endif // MGLDEBUG

#pragma comment(lib, "glfw3.lib")