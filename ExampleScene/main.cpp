#include "Scene.h"

int main() {

	// Create new MGLRenderer
	Scene* scene = new Scene();
	
	while (!scene->ShouldClose()) {
		scene->RenderScene();
	}

	delete scene;
	return 0;
}