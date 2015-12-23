#pragma once
#include "MGL.h"

class Scene : public MGLRenderer {
public:
	Scene();
	~Scene(){};

	void RenderScene() override;

private:
	MGLMeshTextured* triangle;
	MGLShader* shader;
};