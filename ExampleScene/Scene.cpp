#include "Scene.h"

Scene::Scene() : MGLRenderer() {
	CreateNewWindow(800, 600, "MGL Window!", WINDOWTYPE::WINDOWED);
	InitGL();

	triangle = new MGLMeshTextured();
	triangle->GenerateQuad();
	shader = new MGLShader();
	shader->LoadShader("vert.glsl", GL_VERTEX_SHADER);
	shader->LoadShader("frag.glsl", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();
	//MGL::EnableWireframe();
}

void Scene::RenderScene() {
	PollEvents();

	glClear(GL_COLOR_BUFFER_BIT);

	triangle->Draw();

	SwapBuffers();
}