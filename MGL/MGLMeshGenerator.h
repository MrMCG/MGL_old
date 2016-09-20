#pragma once
#include "stdafx.h"

class MGLMesh;

class MGLMeshGenerator{
public:

	static MGLMesh* Triangle();
	static MGLMesh* Quad();
	static MGLMesh* Cone() { return Triangle(); } // TODO
	static MGLMesh* Cube() { return Triangle(); } // TODO
	static MGLMesh* Sphere() { return Triangle(); } // TODO

};
