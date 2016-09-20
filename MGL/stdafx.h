// Pre-Compiled headers
#pragma once

/*
	NOTE: MGLDEBUG is defined in VS properties for debug build
*/

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

/****** FOR MEMORY DEBUGGING *****/
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif

#endif  // _DEBUG

#pragma warning(push, 0) // disable third party/stl warnings

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

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <tiny_obj_loader.h>

#pragma comment(lib, "opengl32.lib")

#ifdef MGLDEBUG
#pragma comment(lib, "libglew32d.lib")
#else
#pragma comment(lib, "libglew32.lib")
#endif // MGLDEBUG

#pragma comment(lib, "glfw3.lib")

#pragma warning(pop) // wnable warnings again