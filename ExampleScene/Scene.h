#pragma once
#include "MGL.h"

class Scene : public MGLRenderer {
public:
	Scene();
	~Scene();

	void RenderScene() override;

	GLfloat GetDeltaTime() const { return deltaTime; }

private:
	void InitInputFuncs();
	void loadObjects();

	MGLMesh* dino;
	MGLMesh* box;
	MGLMesh* floor;

	MGLShader* shader;

	GLfloat lastFrame;
	GLfloat deltaTime;
};

/****** Input functions ******/
void Key_MOUSE_Func(Scene* inputData);
void Key_SCROLL_Func(Scene* inputData);

void Movement_Func(Scene* inputData, MGLenum* funcData);




