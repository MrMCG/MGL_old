#include "Scene.h"
#include <direct.h>

Scene::Scene() : MGLRenderer() {
	CreateNewWindow(800, 600, "MGL Example Window!", MGL_WINDOWTYPE_WINDOWED);
	InitGL();

	triangle = new MGLMeshTextured();
	triangle->GenerateTriangle();
	//triangle->SetColours(MGL::WHITE);
	shader = new MGLShader();
	shader->LoadShader("vert.glsl", GL_VERTEX_SHADER);
	shader->LoadShader("frag.glsl", GL_FRAGMENT_SHADER);
	shader->Link();
	shader->Use();
	//MGL::EnableWireframe();
	
	GLuint texture = MGL::LoadTextureFromFile("bricks.jpg");
	MGL::SetTextureParameters(texture, true, true);

	GLuint bump = MGL::LoadTextureFromFile("stars.jpg");
	MGL::SetTextureParameters(bump, true, true);

	triangle->SetTexture(texture);
	triangle->SetBump(bump);
	triangle->SetUniforms([&]() {
		glUniform1i(glGetUniformLocation(shader->Program(), "tex"), 0);
		glUniform1i(glGetUniformLocation(shader->Program(), "bump"), 1);
	});	
}

void Scene::RenderScene() {
	PollEvents();

	glClear(GL_COLOR_BUFFER_BIT);

	triangle->Draw();

	SwapBuffers();
}