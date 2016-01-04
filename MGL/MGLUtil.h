#pragma once
#include <iostream>
#include "MGLExceptions.h"

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

/****** Typedefs ******/

typedef GLuint MGLenum;
typedef void (*MGLFunction)(void*);

/****** Utilities ******/

// Simple static functions that change state
// ENSURE MGL INIT WAS SUCCESSFUL!
namespace MGL {
	// Enabled wireframe
	static void EnableWireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	// Disables wireframe
	static void DisableWireframe() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

	// Set basic texture params
	// repeat: true = repeat, false = clamp to edge
	// linear: true = min linear mipmap / max linear, false = nearest             
	static void SetTextureParameters(GLuint texture, bool repeat, bool linear) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);	
	}

	// Loads a texture and generates mipmaps
	static GLuint LoadTextureFromFile(std::string fileName) {
		GLuint image = 0;
		try {
			image = SOIL_load_OGL_texture(fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
			MGLException_SOIL::IsSuccessful(image, fileName);
		}
		catch (MGLException& e) {
			std::cerr << e.what() << std::endl;
			return 0;
		}
		return image;
	}

	static int GetWindowInfo(GLFWwindow* window, MGLenum info, GLint attribute = 0) {
		if (!window)
			return 0;

		GLint wanted = 0;
		GLint unw = 0;

		switch (info) {
		case MGL_WINDOWINFO_WIDTH:
			glfwGetWindowSize(window, &wanted, &unw);
			break;
		case MGL_WINDOWINFO_HEIGHT:
			glfwGetWindowSize(window, &unw, &wanted);
			break;
		case MGL_WINDOWINFO_XPOS:
			glfwGetWindowPos(window, &wanted, &unw);
			break;
		case MGL_WINDOWINFO_YPOS:
			glfwGetWindowPos(window, &unw, &wanted);
			break;
		case MGL_WINDOWINFO_ATTRIBUTE:
			wanted = glfwGetWindowAttrib(window, attribute);
			break;
		case MGL_WINDOWINFO_M_XPOS: {
				GLdouble want, unwd;
				glfwGetCursorPos(window, &want, &unwd);
				wanted = (GLint)want;
			}
			break;
		case MGL_WINDOWINFO_M_YPOS: {
				GLdouble want, unwd;
				glfwGetCursorPos(window, &unwd, &want);
				wanted = (GLint)want;
			}
			break;
		}
		return wanted;
	}

	static void PrintMat4(const glm::mat4& matrix) {
		double val[16] = { 0.0 };

		const float *pSource = (const float*)glm::value_ptr(matrix);
		for (int i = 0; i < 16; ++i)
			val[i] = pSource[i];

		std::cout << val[0] << "\t" << val[1] << "\t" << val[2] << "\t" << val[3] << std::endl;
		std::cout << val[4] << "\t" << val[5] << "\t" << val[6] << "\t" << val[7] << std::endl;
		std::cout << val[8] << "\t" << val[9] << "\t" << val[10] << "\t" << val[11] << std::endl;
		std::cout << val[12] << "\t" << val[13] << "\t" << val[14] << "\t" << val[15] << std::endl;
	}

	/****** Useful data ******/

	const static glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const static glm::vec4 BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	const static glm::vec4 RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	const static glm::vec4 GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}