#include "Scene.h"

int main() {

	// Create new MGLRenderer
	Scene* scene = new Scene();
	
	while (!scene->GetWindow()->ShouldClose()) {
		scene->RenderScene();
	}
	
	delete scene;

	_CrtDumpMemoryLeaks();
	return 0;
}