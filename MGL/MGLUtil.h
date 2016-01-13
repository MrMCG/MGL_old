#pragma once
#include "stdafx.h"

/****** Defines ******/

// MGLShader

#define MGL_SHADER_VERTEX 0
#define MGL_SHADER_FRAGMENT 1
#define MGL_SHADER_GEOMETRY 2
#define MGL_SHADER_MAX 3

// MGLMesh

#define MGL_BUFFER_VERTEX 0
#define MGL_BUFFER_TEXTURES 1
#define MGL_BUFFER_NORMALS 2
#define MGL_BUFFER_COLOURS 3
#define MGL_BUFFER_INDICES 4
#define MGL_BUFFER_MAX 5

#define MGL_MESH_QUAD 1
#define MGL_MESH_TRIANGLE 2

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

// MGLFile

#define MGL_FILE_MINSIZE 32
#define MGL_FILE_CURRENTVERSION 1.0f

// MGLTexture

#define MGL_TEXTURE_DIFFUSE 1
#define MGL_TEXTURE_SPECULAR 2

// MGLInput

#define MGL_INPUT_SCROLLACTION 5

// MGLLog

#define MGL_LOG_MAXLINESIZE 512
#define MGL_LOG_DIRECTORY "_LOGS/"
#define MGL_LOG_FILENAME_MAIN MGL_LOG_DIRECTORY"mgl_main_log.txt"
#define MGL_LOG_FILENAME_ERROR MGL_LOG_DIRECTORY"mgl_error_log.txt"
#define MGL_LOG_MAIN GL_TRUE
#define MGL_LOG_ERROR GL_FALSE

// Instance handlers

#define MGLFileHandle MGLFile::Instance()
#define MGLComMeshHandle MGLCommonMeshes::Instance()
#define MGLTexHandle MGLTexture::Instance()
#define MGLLogHandle MGLLog::Instance()

// MGLLog

/****** Other ******/

#define MGL_DEFAULT_DIRECTORY "_DEFAULTS/"
#define MGL_TESTS_DIRECTORY MGL_DEFAULT_DIRECTORY"TESTS/"

#define MGL_DEFAULT_TEXTURE MGL_DEFAULT_DIRECTORY"stars.jpg"

#define MGL_DEFAULT_CUBE MGL_DEFAULT_DIRECTORY"cube.mgl"
#define MGL_DEFAULT_SPHERE MGL_DEFAULT_DIRECTORY"sphere.mgl"
#define MGL_DEFAULT_CONE MGL_DEFAULT_DIRECTORY"cone.mgl"

/****** Typedefs ******/

typedef GLuint MGLenum;
typedef void(*MGLFunction1)(void*);
typedef void(*MGLFunction2)(void*, void*);

typedef std::vector<glm::vec2> MGLvecv2;
typedef std::vector<glm::vec3> MGLvecv3;
typedef std::vector<glm::vec4> MGLvecv4;

typedef std::vector<GLuint> MGLvecu;
typedef std::vector<GLfloat> MGLvecf;

typedef std::vector<std::string> MGLvecs;
typedef std::vector<MGLenum> MGLvecm;


struct MGLObjVertData {
	GLuint vertex = 0;
	GLuint texture = 0;
	GLuint normals = 0;
	GLuint id = 0;

	GLboolean Compare(const MGLObjVertData& o2) {
		if (this->vertex == o2.vertex &&
			this->texture == o2.texture &&
			this->normals == o2.normals)
			return GL_TRUE;

		return GL_FALSE;
	}

	GLboolean operator<(const MGLObjVertData& o) const {
		return this->id < o.id ? GL_TRUE : GL_FALSE;
	}
};

struct MGLObjFileData {
	MGLvecv2 inputTexCoords;
	MGLvecv3 inputVertices;
	MGLvecv3 inputNormals;

	MGLvecu vertexIndices;
	MGLvecu texIndices;
	MGLvecu normalIndices;

	std::vector<MGLObjVertData> objVertexList;
	MGLvecu finalIndices;

	GLuint idCounter = 0;
};

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
	GLuint LoadTextureFromFile(const std::string& fileName, const GLboolean flipY = GL_TRUE);

	int GetWindowInfo(GLFWwindow* window, const MGLenum info, const GLint attribute = 0);
		
	void PrintMat4(const glm::mat4& matrix);

	/****** Useful data ******/

	const glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const glm::vec4 BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	const glm::vec4 RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	const glm::vec4 GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
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