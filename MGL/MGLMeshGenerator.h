#pragma once
#include "stdafx.h"

class MGLMesh;

class MGLMeshGenerator{
public:
	// Generates a triangle mesh
	static MGLMesh* GenerateTriangle();
	// Generates a quad mesh using indices (triangle strip)
	static MGLMesh* GenerateQuad();
};
