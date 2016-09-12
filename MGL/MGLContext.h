#pragma once
#include "stdafx.h"

#include "MGLUtil.h"
#include "MGLWindow.h"
#include "MGLInput.h"
#include "MGLDebug.h"
#include "MGLExceptions.h"
#include "MGLFile.h"
#include "MGLTexture.h"
#include "MGLMesh.h"
#include "MGLShader.h"

// images: http://opengameart.org/content/50-free-textures-4-normalmaps
// http://bgfons.com/upload/stars_texture2944.jpg

class MGLContext {
public:
	MGLContext();
	virtual ~MGLContext();

	virtual void RenderScene() = 0;

	std::shared_ptr<MGLWindow> GetWindow() const { return window; }
	std::shared_ptr<MGLInput> GetInput() const { return input; }

protected:

	virtual void PollEvents() { glfwPollEvents(); input->PollInput(); };
	virtual void SwapBuffers() { glfwSwapBuffers(window->GetGLFWWindow()); }

	virtual void WriteOGLInfo();
	virtual void WriteWindowInfo();
	virtual void WriteDefinesInfo();

	std::shared_ptr<MGLWindow> window;
	std::shared_ptr<MGLInput> input;
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