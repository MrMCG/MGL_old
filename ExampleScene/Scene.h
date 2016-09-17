#pragma once
#include "MGLRenderer.h"

#define testtest

class Scene : public MGLRenderer {
public:
	Scene();
	~Scene();

	void RenderScene() override;

private:
	void InitInputFuncs();
	void loadObjects();
	std::vector<std::thread> threads; // for multi testing

	MGLDataMap<MGLMesh>* meshes;

	MGLShader* shader;
};

/****** Input functions ******/

void Key_MOUSE_Func(Scene* inputData);
void Key_SCROLL_Func(Scene* inputData);
void key_ESC_Func(Scene* inputData);
void Movement_Func(Scene* inputData, MGLenum* funcData);
void windowTest(Scene* inputData);




