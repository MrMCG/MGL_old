#pragma once
#include "MGL.h"



class Scene : public MGLRenderer {
public:
	Scene();
	~Scene();

	void RenderScene() override;

	GLfloat GetDeltaTime() const { return deltaTime; }
	void load();

private:
	void Init();

	MGLMesh* object;
	MGLShader* shader;
	GLfloat lastFrame;
	GLfloat deltaTime;
};

void Key_W_Func(void* data);
void Key_A_Func(void* data);
void Key_S_Func(void* data);
void Key_D_Func(void* data);
void Key_UP_Func(void* data);
void Key_DOWN_Func(void* data);
void Key_MOUSE_Func(void* data);
void Key_SCROLL_Func(void* data);
void Key_1_Func(void* data);
void Key_2_Func(void* data);




