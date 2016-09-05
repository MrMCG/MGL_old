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
#include "MGLInput.h"
#include "MGLTexture.h"
#include "MGLFile.h"
#include "MGLDebug.h"

// images: http://opengameart.org/content/50-free-textures-4-normalmaps
// http://bgfons.com/upload/stars_texture2944.jpg

class MGLContext {
public:
	MGLContext();
	MGLContext(GLuint major, GLuint minor, GLboolean resizable, GLuint refreshRate, GLuint msaa);
	virtual ~MGLContext();

	virtual void RenderScene() = 0;
	virtual void InitGL(); // use AFTER window is linked
	virtual void WriteDefinesInfo();
	// Writes some OpenGL info to main log - !!! uses OGL version > 3.0 !!!
	virtual void WriteOGLInfo();
	virtual void WriteWindowInfo();
	virtual void CreateNewWindow(GLuint width, GLuint height, std::string title, MGLenum windowType, GLFWmonitor* monito = nullptr);
	virtual GLint ShouldClose() { return glfwWindowShouldClose(m_window); }
	virtual void CloseWindow() { glfwSetWindowShouldClose(m_window, GL_TRUE); }
	// Set callback on window resize (if m_resizable=true and window!=NULL)
	void SetResizeCallback(GLFWwindowsizefun func);
	GLFWwindow* GetWindow() const { return m_window; }

protected:

	inline virtual void PollEvents() { glfwPollEvents(); }
	inline virtual void SwapBuffers() { glfwSwapBuffers(m_window); }

	virtual void HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods);
	virtual void HandleMouseButton(GLuint button, GLuint action, GLuint mods);
	virtual void HandleResize(GLuint width, GLuint height);
	virtual void HandleMousePosition(GLdouble xPos, GLdouble yPos);
	virtual void HandleMouseScroll(GLdouble xOffset, GLdouble yOffset);
	virtual void HandleMouseFocus(GLboolean focused);

	void SetWindow(GLFWwindow* win);
	
	static void ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height);
	static void KeyInputCallBack(GLFWwindow* window, GLuint key, GLuint scancode, GLuint action, GLuint mods);
	static void MouseButtonCallBack(GLFWwindow* window, GLuint button, GLuint action, GLuint mods);
	static void MousePositionCallBack(GLFWwindow* window, GLdouble xPos, GLdouble yPos);
	static void MouseScrollCallBack(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset);
	static void MouseFocusCallBack(GLFWwindow* window, GLboolean focused);

	GLFWwindow* m_window;
	GLboolean m_resizable;
	GLuint m_windowType;
	GLuint m_width;
	GLuint m_height;
	GLboolean m_inFocus;
	GLuint m_refreshRate;
	GLuint m_samples;

private:
	void SetHintsGLVersion(GLuint major, GLuint Minor);
	void SetHintsSamples(GLuint samples);
	void SetHints
}; 

class MGLRenderer : public MGLContext {
public:
	MGLRenderer();
	MGLRenderer(GLuint major, GLuint minor, GLboolean resizable, GLuint refreshRate, GLuint msaa);
	virtual ~MGLRenderer();

	virtual void InitGL() override;

	MGLCamera* GetCamera() { return m_camera; }
	MGLMouse* GetMouse() { return m_mouse; }

	/****** TEMP ******/
	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_modelMatrix;
	/****** TEMP ******/

protected:

	virtual void PollEvents();
	virtual void InitInstances();
	virtual void CreateViewMatrix();
	virtual void CreateProjectionMatrix(const GLboolean orthographic = GL_FALSE);

	virtual void HandleResize(GLuint width, GLuint height) override;
	virtual void HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) override;
	virtual void HandleMousePosition(GLdouble xPos, GLdouble yPos) override;
	virtual void HandleMouseButton(GLuint button, GLuint action, GLuint mods) override;
	virtual void HandleMouseFocus(GLboolean focused) override;
	virtual void HandleMouseScroll(GLdouble xOffset, GLdouble yOffset);

	MGLCamera* m_camera;
	MGLKeyboard* m_keyboad;
	MGLMouse* m_mouse;
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