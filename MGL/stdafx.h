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

#include <sstream>
#include <algorithm>