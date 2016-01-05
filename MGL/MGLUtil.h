#pragma once
#include "stdafx.h"

#include <mutex>
#include <iostream>

/****** Defines ******/

// MGLShader

#define MGL_SHADER_VERTEX 0
#define MGL_SHADER_FRAGMENT 1
#define MGL_SHADER_GEOMETRY 2
#define MGL_SHADER_MAX 3

// MGLMesh

#define MGL_BUFFER_VERTEX 0
#define MGL_BUFFER_COLOUR 1
#define MGL_BUFFER_TEXTURE 2
#define MGL_BUFFER_INDICES 3
#define MGL_BUFFER_MAX 4

#define MGL_MESH_QUAD 1
#define MGL_MESH_TRIANGLE 2
#define MGL_MESH_CUBE 3
#define MGL_MESH_SPHERE 4

// MGLenums

#define MGL_NULL 0

#define MGL_WINDOWTYPE_WINDOWED 1
#define MGL_WINDOWTYPE_FULLSCREEN 2
#define MGL_WINDOWTYPE_FULLWINDOWED 3

#define MGL_WINDOWINFO_WIDTH 1
#define MGL_WINDOWINFO_HEIGHT 2
#define MGL_WINDOWINFO_XPOS 3
#define MGL_WINDOWINFO_YPOS 4
#define MGL_WINDOWINFO_ATTRIBUTE 5
#define MGL_WINDOWINFO_M_XPOS 6
#define MGL_WINDOWINFO_M_YPOS 7

#define MGL_CAMERA_FORWARD 0
#define MGL_CAMERA_BACKWARD 1
#define MGL_CAMERA_LEFT 2
#define MGL_CAMERA_RIGHT 3
#define MGL_CAMERA_UP 4
#define MGL_CAMERA_DOWN 5
#define MGL_CAMERA_PITCH 6
#define MGL_CAMERA_YAW 7
#define MGL_CAMERA_ZOOM 8

// MGLTexture

#define MGL_TEXTURE_DIFFUSE 1
#define MGL_TEXTURE_SPECULAR 2

/****** Other ******/

#define MGL_DEFAULT_TEXTURE1 "stars.jpg"
#define MGL_DEFAULT_CUBE "cube.obj"
#define MGL_DEFAULT_SPHERE "sphere.obj"

/****** Typedefs ******/

typedef GLuint MGLenum;
typedef void (*MGLFunction)(void*);

/****** Utilities ******/

// Simple static functions that change state
// ENSURE MGL INIT WAS SUCCESSFUL!
namespace MGL {
	// Enabled wireframe
	inline void EnableWireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glDisable(GL_CULL_FACE); }
	// Disables wireframe
	inline void DisableWireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glEnable(GL_CULL_FACE); }

	// Set basic texture params
	// repeat: true = repeat, false = clamp to edge
	// linear: true = min linear mipmap / max linear, false = nearest             
	void SetTextureParameters(GLuint texture, GLboolean repeat, GLboolean linear);

	// Loads a texture and generates mipmaps
	GLuint LoadTextureFromFile(std::string fileName, GLboolean flipY = GL_TRUE);

	int GetWindowInfo(GLFWwindow* window, MGLenum info, GLint attribute = 0);
		
	void PrintMat4(const glm::mat4& matrix);

	/****** Useful data ******/

	const static glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const static glm::vec4 BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	const static glm::vec4 RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	const static glm::vec4 GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

template<class T>
class MGLSingleton {
public:
	static inline T* Instance() { return m_instance; }
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
	/* Prevent copy */

	MGLSingleton(MGLSingleton const&){}
	MGLSingleton& operator=(MGLSingleton const&){}

	/* Keep instance pointer for refrence */

	static std::mutex m_init;
	static T* m_instance;
};

template <class T> std::mutex MGLSingleton<T>::m_init;
template <class T> T* MGLSingleton<T>::m_instance = nullptr;