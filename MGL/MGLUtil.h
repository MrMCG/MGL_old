#pragma once
#include "stdafx.h"

// include MGL_TESTS_ 
#define MGL_USER_INCLUDE_TESTS
// include try-catch blocks in MGLFileTEMP
#define MGL_USER_INCLUDE_FILETC

// MGLenums

#define MGL_WINDOWINFO_WIDTH 1
#define MGL_WINDOWINFO_HEIGHT 2
#define MGL_WINDOWINFO_XPOS 3
#define MGL_WINDOWINFO_YPOS 4
#define MGL_WINDOWINFO_ATTRIBUTE 5
#define MGL_WINDOWINFO_M_XPOS 6
#define MGL_WINDOWINFO_M_YPOS 7

/****** Other ******/

#define MGL_DEFAULT_DIRECTORY "_DEFAULTS/"
#define MGL_DEFAULT_TESTS_DIRECTORY MGL_DEFAULT_DIRECTORY"TESTS/"

#define MGL_DEFAULT_TEXTURE MGL_DEFAULT_DIRECTORY"mgl_default_tex.png"
#define MGL_DEFAULT_FONT MGL_DEFAULT_DIRECTORY"mgl_default_font.png"

#define MGL_DEFAULT_CUBE MGL_DEFAULT_DIRECTORY"cube.mgl"
#define MGL_DEFAULT_SPHERE MGL_DEFAULT_DIRECTORY"sphere.mgl"
#define MGL_DEFAULT_CONE MGL_DEFAULT_DIRECTORY"cone.mgl"

/****** Typedefs ******/

typedef GLuint MGLenum; // MGLEnum meerly indicates a MGL_ defined variable
typedef void(*MGLFunction1)(void*);
typedef void(*MGLFunction2)(void*, void*);

typedef std::vector<glm::vec2> MGLvecv2;
typedef std::vector<glm::vec3> MGLvecv3;
typedef std::vector<glm::vec4> MGLvecv4;

typedef std::vector<GLuint> MGLvecu;
typedef std::vector<GLfloat> MGLvecf;

typedef std::vector<std::string> MGLvecs;
typedef std::vector<MGLenum> MGLvecm;

/****** Structs ******/



/****** Utilities ******/

// Simple utility functions
// ENSURE MGL INIT (GLFW + GLEW) WAS SUCCESSFUL!
namespace MGL {
	// Enabled wireframe
	void EnableWireframe();
	// Disables wireframe
	void DisableWireframe();
	// Set basic texture params      
	void SetTextureParameters(const GLuint texture, const GLboolean repeat, const GLboolean linear);
	// Loads a texture and generates mipmaps
	GLuint LoadTextureFromFile(const std::string& fileName, const GLboolean alpha = GL_FALSE, const GLboolean flipY = GL_TRUE);
	// Get window info or attributes
	int GetWindowInfo(GLFWwindow* window, const MGLenum info, const GLint attribute = 0);
	// Print glm::mat4, uses std::cout
	void PrintMat4(const glm::mat4& matrix);

	std::size_t GetFilesize(std::ifstream& file);
	GLboolean FileOpenedSuccessful(std::ifstream& file, std::string fileName);

	/****** Useful data ******/

	const glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const glm::vec4 BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	const glm::vec4 RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	const glm::vec4 GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

template<typename T>
class MGLSingleton {
public:
	static T* Instance() { return m_instance; }
	static void Init() {
		if (!m_instance) {
			std::lock_guard<std::mutex> lock(m_init);
			if (!m_instance) {
				m_instance = new T();
			}
		}
	};
	static void Release() {
		if (m_instance) {
			std::lock_guard<std::mutex> lock(m_init);
			if (m_instance) {
				delete m_instance;
				m_instance = nullptr;
			}
		}
	};

protected:
	MGLSingleton(){}
	~MGLSingleton(){}

private:
	/* Prevent copy/move */

	MGLSingleton(const MGLSingleton&) = delete;
	MGLSingleton(const MGLSingleton&&) = delete;
	MGLSingleton& operator=(const MGLSingleton&) = delete;
	MGLSingleton& operator=(const MGLSingleton&&) = delete;

	/* Keep instance pointer for refrence */

	static std::mutex m_init;
	static T* m_instance;
};

template <typename T> std::mutex MGLSingleton<T>::m_init;
template <typename T> T* MGLSingleton<T>::m_instance = nullptr;