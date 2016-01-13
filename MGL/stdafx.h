// Pre-Compiled headers
#pragma once

/*
	NOTE: MGLDEBUG is defined in VS properties for debug build

	NOTE: GLFW is built using MSVC_RUNTIME_LIBRARY_DLL
*/

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC

// Third Party

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "stb_image.h"

// MGL

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