#include "Scene.h"
//#include <direct.h>

// 1. derive from MGL or MGLRenderer
Scene::Scene() : MGLRenderer() {

	// 2. Build window

	// 3. Init GL features and Instances

	// 4. Create your scene!


	// Create new shader program
	shader = new MGLShader();
	shader->LoadShader("shaders/vert.glsl", GL_VERTEX_SHADER);
	shader->LoadShader("shaders/frag.glsl", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();

	// Load new texture into handler
	MGLH_Tex->LoadTexture("textures/raptor.jpg", "raptor", MGL_TEXTURE_DIFFUSE, GL_FALSE);
	MGLH_Tex->LoadTexture("textures/cty1.jpg", "city", MGL_TEXTURE_DIFFUSE, GL_TRUE);
	MGLH_Tex->LoadTexture("textures/ds.jpg", "death star", MGL_TEXTURE_DIFFUSE, GL_FALSE);

	// set camera options
	camera->SetMoveSpeed(100.0f);
	camera->SetPosition(glm::vec3(0,20,0));

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
	delete city;
	delete shader;
}

void Scene::loadObjects() {
	dino = MGLH_FileMGL->Load("meshes/raptor.mgl");
	// Load raptor
	//dino = MGLFileHandle->Load("meshes/raptor.mgl");
	dino->AddTexture(MGLH_Tex->GetTexture("raptor"));

	dino->SetUniforms([&]() { // just as an example
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
	});

	// Load box
	box = MGLH_ComMesh->Cube();
	box->AddTexture(MGLH_Tex->GetTexture("DEFAULT"));

	box->SetUniforms([&]() { // just as an example
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
	});

	// Load city
	city = MGLH_FileOBJ->Load("meshes/The City.obj");
	city->AddTexture(MGLH_Tex->GetTexture("city"));

	city->SetUniforms([&]() { // just as an example
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
	});

	// Load death star
	deathStar = MGLH_FileMGL->Load("meshes/death-star-II.mgl");
	deathStar->AddTexture(MGLH_Tex->GetTexture("death star"));

	deathStar->SetUniforms([&]() { // just as an example
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
	});
}

void Scene::InitInputFuncs() {
	// Add movements keyboard functions to keys
	input->AddKeyboardFunction(GLFW_KEY_W, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_FORWARD));
	input->AddKeyboardFunction(GLFW_KEY_A, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_LEFT));
	input->AddKeyboardFunction(GLFW_KEY_S, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_BACKWARD));
	input->AddKeyboardFunction(GLFW_KEY_D, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_RIGHT));
	input->AddKeyboardFunction(GLFW_KEY_UP, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_UP));
	input->AddKeyboardFunction(GLFW_KEY_DOWN, GLFW_REPEAT, GLFW_PRESS, (MGLFunction2)Movement_Func, new MGLenum(MGL_CAMERA_DOWN));
	
	input->AddKeyboardFunction(GLFW_KEY_Y, GLFW_PRESS, (MGLFunction2)windowTest, nullptr);

	input->AddKeyboardFunction(GLFW_KEY_1, GLFW_PRESS, (MGLFunction2)MGL::EnableWireframe, nullptr);
	input->AddKeyboardFunction(GLFW_KEY_2, GLFW_PRESS, (MGLFunction2)MGL::DisableWireframe, nullptr);
	input->AddKeyboardFunction(GLFW_KEY_ESCAPE, GLFW_PRESS, (MGLFunction2)key_ESC_Func, nullptr);

	// Add mouse functions
	input->AddMouseFunction(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, (MGLFunction2)Key_MOUSE_Func, nullptr);
	input->AddScrollFunction((MGLFunction2)Key_SCROLL_Func, nullptr);

	// Set user data pointers for access in functions
	input->SetDataPointer(this);
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
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "viewMatrix"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "projMatrix"), 1, false, glm::value_ptr(projMatrix));

	// Draw city
	modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -5.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "modelMatrix"), 1, false, glm::value_ptr(modelMatrix));
	city->Draw();

	// Draw dino
	modelMatrix = glm::translate(glm::mat4(), glm::vec3(150.0f, 106.0f, -100.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0, -1, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "modelMatrix"), 1, false, glm::value_ptr(modelMatrix));
	dino->Draw();

	// Draw box
	modelMatrix = glm::translate(glm::mat4(), glm::vec3(5.0f, 0.0f, -5.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0, -1, 0));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "modelMatrix"), 1, false, glm::value_ptr(modelMatrix));
	box->Draw();

	// Draw death star
	modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 750.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "modelMatrix"), 1, false, glm::value_ptr(modelMatrix));
	deathStar->Draw();

	// Swap buffers
	SwapBuffers();

	// End frame time
	gameTimer.End();
}

void Movement_Func(Scene* inputData, MGLenum* funcData) {
	inputData->GetCamera()->MoveCamera(*funcData, inputData->GetDeltaTime());
}

void Key_MOUSE_Func(Scene* inputData) {
	inputData->GetCamera()->MoveCamera(MGL_CAMERA_PITCH, inputData->GetInput()->GetMouseOffsetY());
	inputData->GetCamera()->MoveCamera(MGL_CAMERA_YAW, inputData->GetInput()->GetMouseOffsetX());
}

void Key_SCROLL_Func(Scene* inputData) {
	inputData->GetCamera()->MoveCamera(MGL_CAMERA_ZOOM, inputData->GetInput()->GetScrollY());
}

void key_ESC_Func(Scene* inputData) {
	inputData->GetWindow()->CloseWindow();
}

void windowTest(Scene* inputData) {
	inputData->GetWindow()->SetWindowSize(1000, 800);
}