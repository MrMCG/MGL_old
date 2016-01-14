#include "Scene.h"
//#include <direct.h>

Scene::Scene() : MGLRenderer() {
	// Create window context
	CreateNewWindow(800, 600, "MGL Example Window!", MGL_WINDOWTYPE_WINDOWED);
	// Init GLEW and other GL features
	InitGL();

#ifdef MGLDEBUG
	// For running test cases
	MGL_TESTS_::MGL_TEST_CLASSES();
	//MGL_TESTS_::MGL_TEST_VARIOUS();
#endif MGLDEBUG

	// Create new shader program
	shader = new MGLShader();
	shader->LoadShader("shaders/vert.glsl", GL_VERTEX_SHADER);
	shader->LoadShader("shaders/frag.glsl", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();

	// Load new texture into handler
	MGLTexHandle->LoadTexture("textures/raptor.jpg", "raptor", true, true);
	MGLTexHandle->LoadTexture("textures/bricks.jpg", "bricks", true, true);

	// Add a camera
	m_camera = new MGLCamera();

	// Create new timer
	gameTimer = MGLTimer();

	// Init delta time variables
	deltaTime = 0.0f;

	// load objects
	loadObjects();

	// Set input commands
	InitInputFuncs();
}

Scene::~Scene() {
	// NOTE: we dont delete box, as it is a MGLCommonMesh!
	delete dino;
	delete floor;
	delete shader;
}

void Scene::loadObjects() {
	// Load raptor
	dino = MGLFileHandle->LoadMGL("meshes/raptor.mgl");
	dino->AddTexture(MGLTexHandle->GetTexture("raptor"));

	dino->SetUniforms([&]() { // just as an example
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
	});

	// Load box
	box = MGLComMeshHandle->Cube();
	box->AddTexture(MGLTexHandle->GetTexture("DEFAULT"));

	box->SetUniforms([&]() { // just as an example
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
	});

	// Load flooar
	floor = new MGLMesh(MGL_MESH_QUAD);
	floor->AddTexture(MGLTexHandle->GetTexture("bricks"));

	floor->SetUniforms([&]() { // just as an example
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
	});
}

void Scene::InitInputFuncs() {
	// Add movements keyboard functions to keys
	m_keyboad->AddKeyFunction(GLFW_KEY_W, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_FORWARD));
	m_keyboad->AddKeyFunction(GLFW_KEY_A, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_LEFT));
	m_keyboad->AddKeyFunction(GLFW_KEY_S, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_BACKWARD));
	m_keyboad->AddKeyFunction(GLFW_KEY_D, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_RIGHT));
	m_keyboad->AddKeyFunction(GLFW_KEY_UP, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_UP));
	m_keyboad->AddKeyFunction(GLFW_KEY_DOWN, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_DOWN));

	m_keyboad->AddKeyFunction(GLFW_KEY_1, GLFW_PRESS, (MGLFunction2)MGL::EnableWireframe, nullptr);
	m_keyboad->AddKeyFunction(GLFW_KEY_2, GLFW_PRESS, (MGLFunction2)MGL::DisableWireframe, nullptr);
	m_keyboad->AddKeyFunction(GLFW_KEY_ESCAPE, GLFW_PRESS, (MGLFunction2)key_ESC_Func, nullptr);

	// Add mouse functions
	m_mouse->AddKeyFunction(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, (MGLFunction2)Key_MOUSE_Func, nullptr);
	m_mouse->AddScrollFunction((MGLFunction2)Key_SCROLL_Func, nullptr);

	// Set user data pointers for access in functions
	m_keyboad->SetDataPointer(this);
	m_mouse->SetDataPointer(this);
}

void Scene::RenderScene() {
	// Get frame time
	deltaTime = gameTimer.Time();
	//std::cout << deltaTime << std::endl;

	// Start new frame time
	gameTimer.Start();

	// Poll input
	PollEvents();

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update Matrices
	CreateProjectionMatrix();
	CreateViewMatrix();

	// Set uniforms
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "viewMatrix"), 1, false, glm::value_ptr(m_viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "projMatrix"), 1, false, glm::value_ptr(m_projMatrix));

	// Draw floor
	m_modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -5.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(90.0f), glm::vec3(-1, 0, 0));
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(25.0f, 25.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "modelMatrix"), 1, false, glm::value_ptr(m_modelMatrix));
	floor->Draw();

	// Draw dino
	m_modelMatrix = glm::translate(glm::mat4(), glm::vec3(-5.0f, 0.0f, -5.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(90.0f), glm::vec3(0, -1, 0));
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "modelMatrix"), 1, false, glm::value_ptr(m_modelMatrix));
	dino->Draw();

	// Draw box
	m_modelMatrix = glm::translate(glm::mat4(), glm::vec3(5.0f, 0.0f, -5.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(90.0f), glm::vec3(0, -1, 0));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "modelMatrix"), 1, false, glm::value_ptr(m_modelMatrix));
	box->Draw();

	// Swap buffers
	SwapBuffers();

	// End frame time
	gameTimer.End();
}

void Movement_Func(Scene* inputData, MGLenum* funcData) {
	inputData->GetCamera()->MoveCamera(*funcData, inputData->GetDeltaTime());
}

void Key_MOUSE_Func(Scene* inputData) {
	if (inputData->GetMouse()->GetHasUpdated()) { // check if camera has moved
		inputData->GetCamera()->MoveCamera(MGL_CAMERA_PITCH, inputData->GetMouse()->GetOffsetY());
		inputData->GetCamera()->MoveCamera(MGL_CAMERA_YAW, inputData->GetMouse()->GetOffsetX());
		inputData->GetMouse()->SetHasUpdated(GL_FALSE); // reset for next update
	}
}

void Key_SCROLL_Func(Scene* inputData) {
	inputData->GetCamera()->MoveCamera(MGL_CAMERA_ZOOM, inputData->GetMouse()->GetScrollY() * 0.1f);
	inputData->GetMouse()->SetScrollUpdated(GL_FALSE); // reset for next scroll
}

void key_ESC_Func(Scene* inputData) {
	inputData->CloseWindow();
}