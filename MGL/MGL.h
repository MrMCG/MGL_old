#pragma once
#include "stdafx.h"

#pragma comment(lib, "opengl32.lib")

#ifdef MGLDEBUG
#pragma comment(lib, "glew32sd.lib")
#else
#pragma comment(lib, "glew32s.lib")
#endif // MGLDEBUG

#pragma comment(lib, "glfw3.lib")

#include "MGLWindow.h"

#include "MGLUtil.h"
#include "MGLExceptions.h"
#include "MGLDebug.h"

#include "MGLMesh.h"
#include "MGLShader.h"
#include "MGLCamera.h"
#include "MGLTexture.h"
#include "MGLFile.h"
#include "MGLDebug.h"

// images: http://opengameart.org/content/50-free-textures-4-normalmaps
// http://bgfons.com/upload/stars_texture2944.jpg

class MGLContext {
public:
	MGLContext();
	virtual ~MGLContext();

	virtual void RenderScene() = 0;

	// Writes some OpenGL info to main log - !!! uses OGL version > 3.0 !!!
	virtual void WriteOGLInfo();
	virtual void WriteWindowInfo();
	virtual void WriteDefinesInfo();  

	MGLWindow* GetWindow() const { return window; }

protected:

	virtual void PollEvents() { glfwPollEvents(); window->PollInput(); };
	virtual void SwapBuffers() { glfwSwapBuffers(window->GetGLFWWindow()); }

	MGLWindow* window;
}; 

class MGLRenderer : public MGLContext {
public:
	MGLRenderer();
	virtual ~MGLRenderer();

	MGLCamera* GetCamera() { return m_camera; }
	
	/****** TEMP ******/
	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_modelMatrix;
	/****** TEMP ******/

protected:

	virtual void InitInstances();
	virtual void CreateViewMatrix();
	virtual void CreateProjectionMatrix(const GLboolean orthographic = GL_FALSE);

	MGLCamera* m_camera;
};



#ifdef MGL_USER_INCLUDE_TESTS
// Test cases for debugging
namespace MGL_TESTS_ {
	
	/****** Utility functions *****/

	// Deletes files created by test cases
	void MGL_FileCleanup();

	// Runs all tests
	void MGL_TEST_ALL();

	/****** Various Tests *****/

	// Run ALL various tests
	void MGL_TEST_VARIOUS();
	// Tests load speed of meshes of .obj and .mgl type (takes a while)
	void MGL_TEST_VARIOUS_FILESPEED();
	// Tests memory usage by creating and deleting thousands of objects
	void MGL_TEST_VARIOUS_MEMMORY();

	/****** MGL Classes Test *****/

	// Run ALL class funcitons and clean (delete) created files
	void MGL_TEST_CLASSES(const GLboolean cleanFiles = GL_TRUE);

	GLuint MGL_TEST_MGLFILE();
	GLuint MGL_TEST_MGLFILE_OBJ(); // Load OBJ files
	GLuint MGL_TEST_MGLFILE_MGL(); // Load MGL files
	GLuint MGL_TEST_MGLFILE_SMTM(); // save mesh to .mgl file

	// List of files created by tests, to be destroyed by MGL_FileCleanup
	const MGLvecs createdTestFiles = { // KEEP SIZE EVEN!
		MGL_DEFAULT_TESTS_DIRECTORY"MGLFILE_SMTM_1", // 0
		".mgl",
		MGL_DEFAULT_TESTS_DIRECTORY"MGLFILE_SMTM_2", // 2
		".mgl",
	};
}
#endif // MGL_USER_INCLUDE_TESTS