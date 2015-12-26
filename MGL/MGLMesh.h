#include "stdafx.h"
#pragma once

#include <functional>

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
	// Sets m_vertices amount of colours to specified colour
	void SetColours(glm::vec4 colour);

protected:
	// Loads VBO data into memory
	void BufferData();

	GLuint m_VAO;
	GLuint m_VBO[MGL_BUFFER_MAX];

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

	// Set texture
	void SetTexture(GLuint tex) { m_tex = tex; }
	// Set Bump
	void SetBump(GLuint tex) { m_bump = tex; }

	// Get texture
	GLuint GetTexture() const { return m_tex; }
	// Get bu,p
	GLuint GetBump() const { return m_bump; }

	// User defined unfiforms will be set before drawing
	void SetUniforms(std::function<void(void)> onDrawCallBack) { m_OnDrawCallBack = onDrawCallBack; }

protected:
	GLuint m_tex;
	GLuint m_bump;

	std::function<void(void)> m_OnDrawCallBack;
};