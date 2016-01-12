#pragma once
#include "stdafx.h"

#pragma comment(lib, "opengl32.lib")

#ifdef MGLDEBUG
#pragma comment(lib, "glew32sd.lib")
#else
#pragma comment(lib, "glew32s.lib")
#endif

#pragma comment(lib, "glfw3.lib")

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
	// Defaults: MGL(3,3,true)
	MGLContext();
	MGLContext(GLuint major, GLuint minor, GLboolean resizable = GL_TRUE);
	virtual ~MGLContext();

	// Render loop
	virtual void RenderScene() = 0;
	// Init openGL options (use AFTER window is linked)
	virtual void InitGL();
	// Create a new window for use 
	virtual void CreateNewWindow(GLuint width, GLuint height, std::string title, MGLenum windowType, GLFWmonitor* monito = nullptr);
	// Should the window close
	virtual GLint ShouldClose() { return glfwWindowShouldClose(m_window); }
	// Set callback on window resize (if m_resizable=true and window!=NULL)
	void SetResizeCallback(GLFWwindowsizefun func);

	void SetWindow(GLFWwindow* win);
	GLFWwindow* GetWindow() const { return m_window; }

protected:

	/****** Methods ******/

	// Polls events
	inline virtual void PollEvents() { glfwPollEvents(); }
	// Swaps screen buffers
	inline virtual void SwapBuffers() { glfwSwapBuffers(m_window); }
	// Handle keyboard input
	virtual void HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods);
	// Handle mouse button input
	virtual void HandleMouseButton(GLuint button, GLuint action, GLuint mods);
	// Handle window resize
	virtual void HandleResize(GLuint width, GLuint height);
	// Handle mouse position
	virtual void HandleMousePosition(GLdouble xPos, GLdouble yPos);
	// Handle mouse scroll
	virtual void HandleMouseScroll(GLdouble xOffset, GLdouble yOffset);
	// Handle window.mouse focus
	virtual void HandleMouseFocus(GLboolean focused);

	static void ResizeCallBack(GLFWwindow* window, GLuint width, GLuint height);
	static void KeyInputCallBack(GLFWwindow* window, GLuint key, GLuint scancode, GLuint action, GLuint mods);
	static void MouseButtonCallBack(GLFWwindow* window, GLuint button, GLuint action, GLuint mods);
	static void MousePositionCallBack(GLFWwindow* window, GLdouble xPos, GLdouble yPos);
	static void MouseScrollCallBack(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset);
	static void MouseFocusCallBack(GLFWwindow* window, GLboolean focused);

	/****** Data ******/

	GLFWwindow* m_window;
	GLboolean m_resizable;
	GLuint m_width;
	GLuint m_height;
	GLboolean m_inFocus;
}; 

class MGLRenderer : public MGLContext {
public:
	MGLRenderer();
	virtual ~MGLRenderer();

	// Init openGL options (use AFTER window is linked)
	virtual void InitGL() override;

	MGLCamera* GetCamera() { return m_camera; }
	MGLMouse* GetMouse() { return m_mouse; }

	/****** TEMP ******/
	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_modelMatrix;
	/****** TEMP ******/

protected:

	/****** Methods ******/

	// Polls events (including user mouse and keyboard)
	virtual void PollEvents();
	// Init singleton instances
	virtual void InitInstances();

	// Handlers for window 

	virtual void HandleResize(GLuint width, GLuint height) override;
	virtual void HandleKeyInput(GLuint key, GLuint scancode, GLuint action, GLuint mods) override;
	virtual void HandleMousePosition(GLdouble xPos, GLdouble yPos) override;
	virtual void HandleMouseButton(GLuint button, GLuint action, GLuint mods) override;
	virtual void HandleMouseFocus(GLboolean focused) override;
	virtual void HandleMouseScroll(GLdouble xOffset, GLdouble yOffset);

	/****** Data ******/

	MGLCamera* m_camera;
	MGLKeyboard* m_keyboad;
	MGLMouse* m_mouse;
};

// Test cases for debugging
#ifdef MGLDEBUG
namespace MGL_TESTS_ {
	// Test all funcitons and clean (delete) created files
	void MGL_TEST_ALL(const GLboolean cleanFiles = GL_TRUE);
	// Deletes files created by test cases
	void MGL_TEST_FileCleanup();

	GLuint MGL_TEST_MGLFILE();
	GLuint MGL_TEST_MGLFILE_COTM(); // ConvertOBJToMGL 6
	GLuint MGL_TEST_MGLFILE_LO(); // LadOBJ 10
	GLuint MGL_TEST_MGLFILE_LM(); // LadMGL 18
	GLuint MGL_TEST_MGLFILE_SMTM(); // SaveMeshToMGL 6

	const MGLvecs createdTestFiles = { // KEEP SIZE EVEN!
		MGL_TESTS_DIRECTORY"MGLFILE_LO_1", // 0
		".mgl",
		MGL_TESTS_DIRECTORY"MGLFILE_LO_2", // 2
		".mgl",
		MGL_TESTS_DIRECTORY"MGLFILE_SMTM_1", // 4
		".mgl",
		MGL_TESTS_DIRECTORY"MGLFILE_SMTM_2", // 6
		".mgl",
	};

	const enum Counters{ total, MGLFile, max };
	const enum FuncMax{
		MGLFileMax = 40
	};
}
#endif