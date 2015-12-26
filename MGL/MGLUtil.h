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

// MGL

#define MGL_NULL 0

#define MGL_WINDOWTYPE_WINDOWED 1
#define MGL_WINDOWTYPE_FULLSCREEN 2
#define MGL_WINDOWTYPE_FULLWINDOWED 3

#define MGL_WINDOWINFO_WIDTH 1
#define MGL_WINDOWINFO_HEIGHT 2
#define MGL_WINDOWINFO_XPOS 3
#define MGL_WINDOWINFO_YPOS 4
#define MGL_WINDOWINFO_ATTRIBUTE 5

/****** Typedefs ******/

typedef GLuint MGLenum;

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
		}
		return wanted;
	}

	/****** Useful data ******/

	const static glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const static glm::vec4 BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	const static glm::vec4 RED = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	const static glm::vec4 GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}