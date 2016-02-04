#include "Scene.h"

int main() {

	int list[10];
	int* p;

	p = list;
	for (int x = 1; x <= 10; x++) {
		*(p) = x;
		p++;
		std::cout << &p << std::endl;
	}


	// Create new MGLRenderer
	//Scene* scene = new Scene();
	
	//while (!scene->ShouldClose()) {
		//scene->RenderScene();
	//}
	
	//delete scene;

	//_CrtDumpMemoryLeaks();
	return 0;
}