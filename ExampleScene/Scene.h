#pragma once
#include "MGL.h"

#define testtest

class Scene : public MGLRenderer {
public:
	Scene();
	~Scene();

	void RenderScene() override;

	GLfloat GetDeltaTime() const { return deltaTime; }

private:
	void InitInputFuncs();
	void loadObjects();
	std::vector<std::thread> threads; // for multi testing

	MGLMesh* dino;
	MGLMesh* box;
	MGLMesh* city;
	MGLMesh* deathStar;

	MGLShader* shader;

	MGLTimer gameTimer;

	GLfloat deltaTime;
};

/****** Input functions ******/

void Key_MOUSE_Func(Scene* inputData);
void Key_SCROLL_Func(Scene* inputData);
void key_ESC_Func(Scene* inputData);

void Movement_Func(Scene* inputData, MGLenum* funcData);




