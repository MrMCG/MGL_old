#include "Scene.h"
//#include <direct.h>

Scene::Scene() : MGLRenderer() {
	CreateNewWindow(800, 600, "MGL Example Window!", MGL_WINDOWTYPE_WINDOWED);
	InitGL();

	shader = new MGLShader();
	shader->LoadShader("vert.glsl", GL_VERTEX_SHADER);
	shader->LoadShader("frag.glsl", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();

	//MGLFileHandle->ConvertOBJToMGL("victory-destroyer.obj");

	MGLTexHandle->LoadTexture("raptor.jpg", "bricks", true, true);

	m_camera = new MGLCamera();
	//m_camera->SetMoveSpeed(100.0f);
	m_modelMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f,0.0f,0.0f));

	lastFrame = (GLfloat)glfwGetTime();
	deltaTime = 0.0f;

	load();
	Init();
}

void Scene::load() {
	object = MGLFileHandle->LoadMGL("raptor.mgl");
	object->AddTexture(MGLTexHandle->GetTexture("bricks"));
	object->AddTexture(MGLTexHandle->GetTexture("DEFAULT"));

	object->SetUniforms([&]() {
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
		glUniform1i(glGetUniformLocation(shader->Program(), "bump"), 1);
	});
}

void Scene::Init() {
	m_keyboad->AddKeyFunction(GLFW_KEY_W, GLFW_REPEAT, GLFW_PRESS, Key_W_Func);
	m_keyboad->AddKeyFunction(GLFW_KEY_A, GLFW_REPEAT, GLFW_PRESS, Key_A_Func);
	m_keyboad->AddKeyFunction(GLFW_KEY_S, GLFW_REPEAT, GLFW_PRESS, Key_S_Func);
	m_keyboad->AddKeyFunction(GLFW_KEY_D, GLFW_REPEAT, GLFW_PRESS, Key_D_Func);
	m_keyboad->AddKeyFunction(GLFW_KEY_UP, GLFW_REPEAT, GLFW_PRESS, Key_UP_Func);
	m_keyboad->AddKeyFunction(GLFW_KEY_DOWN, GLFW_REPEAT, GLFW_PRESS, Key_DOWN_Func);

	m_keyboad->AddKeyFunction(GLFW_KEY_1, GLFW_PRESS, Key_1_Func);
	m_keyboad->AddKeyFunction(GLFW_KEY_2, GLFW_PRESS, Key_2_Func);

	m_mouse->AddKeyFunction(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, Key_MOUSE_Func);
	m_mouse->AddScrollFunction(Key_SCROLL_Func);

	m_keyboad->SetDataPointer(this);
	m_mouse->SetDataPointer(this);
}

void Scene::RenderScene() {
	GLfloat currentFrame = (GLfloat)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	PollEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_projMatrix = glm::perspective(m_camera->GetZoom(), 800.0f / 600.0f, 0.1f, 1000.0f);
	m_viewMatrix = m_camera->BuildViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "modelMatrix"), 1, false, glm::value_ptr(m_modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "viewMatrix"), 1, false, glm::value_ptr(m_viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program(), "projMatrix"), 1, false, glm::value_ptr(m_projMatrix));
	object->Draw();

	SwapBuffers();
}

void Key_W_Func(void* data) {
	Scene* game = static_cast<Scene*>(data);
	game->GetCamera()->MoveCamera(MGL_CAMERA_FORWARD, game->GetDeltaTime());
}

void Key_A_Func(void* data) {
	Scene* game = static_cast<Scene*>(data);
	game->GetCamera()->MoveCamera(MGL_CAMERA_LEFT, game->GetDeltaTime());
}

void Key_S_Func(void* data) {
	Scene* game = static_cast<Scene*>(data);
	game->GetCamera()->MoveCamera(MGL_CAMERA_BACKWARD, game->GetDeltaTime());
}

void Key_D_Func(void* data) {
	Scene* game = static_cast<Scene*>(data);
	game->GetCamera()->MoveCamera(MGL_CAMERA_RIGHT, game->GetDeltaTime());
}

void Key_UP_Func(void* data) {
	Scene* game = static_cast<Scene*>(data);
	game->GetCamera()->MoveCamera(MGL_CAMERA_UP, game->GetDeltaTime());
}

void Key_DOWN_Func(void* data) {
	Scene* game = static_cast<Scene*>(data);
	game->GetCamera()->MoveCamera(MGL_CAMERA_DOWN, game->GetDeltaTime());
}

void Key_MOUSE_Func(void* data) {
	Scene* game = static_cast<Scene*>(data);
	if (game->GetMouse()->GetHasUpdated()) {
		game->GetCamera()->MoveCamera(MGL_CAMERA_PITCH, game->GetMouse()->GetOffsetY());
		game->GetCamera()->MoveCamera(MGL_CAMERA_YAW, game->GetMouse()->GetOffsetX());
		game->GetMouse()->SetHasUpdated(GL_FALSE);
	}
}

void Key_SCROLL_Func(void* data) {
	Scene* game = static_cast<Scene*>(data);
	game->GetCamera()->MoveCamera(MGL_CAMERA_ZOOM, game->GetMouse()->GetScrollY() * 0.1f);
	game->GetMouse()->SetScrollUpdated(GL_FALSE);
}

void Key_1_Func(void* data) {
	MGL::EnableWireframe();
}

void Key_2_Func(void* data) {
	MGL::DisableWireframe();
}
