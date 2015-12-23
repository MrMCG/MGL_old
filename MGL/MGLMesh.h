#include "stdafx.h"
#pragma once

class MGLMesh {
public:
	MGLMesh();
	virtual ~MGLMesh();

	// Draws the mesh
	virtual void Draw();

	// Generates a triangle mesh
	void GenerateTriangle();
	// Generates a quad mesh using indices (triangle strip)
	void GenerateQuad();

protected:
	// Loads VBO data into memory
	void BufferData();

	GLuint m_VAO;
	GLuint m_VBO[BUFFER_MAX];

	GLuint m_type;
	GLuint m_numIndices;
	GLuint m_numVertices;

	GLuint* m_indices;

	glm::vec3* m_vertices;
	glm::vec4* m_colours;
	glm::vec2* m_texCoords;
	glm::vec3* m_normals;
	glm::vec3* m_tangents;

};

class MGLMeshTextured : public MGLMesh {
public:
	MGLMeshTextured();
	virtual ~MGLMeshTextured(){};

	// Draws the mesh
	virtual void Draw();

protected:
	GLuint tex;
};