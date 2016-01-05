#pragma once
#include "stdafx.h"

#include "MGLTexture.h"

#include <functional>
#include <vector>

class MGLMesh {
public:
	MGLMesh(MGLenum mgl_mesh_type = 0);
	virtual ~MGLMesh();

	// Draws the mesh
	virtual void Draw();

	// Generates a triangle mesh
	void GenerateTriangle();
	// Generates a quad mesh using indices (triangle strip)
	void GenerateQuad();

	// Sets m_vertices amount of colours to specified colour
	void SetColours(glm::vec4 colour, GLboolean buffer = GL_TRUE);

	// Set texture
	inline void AddTexture(GLuint tex) { m_textures->push_back(tex); }
	// Set Bump
	void RemoveTexture(GLuint tex);

	/* Currently only uses vertices/texCoord/normals */
	static MGLMesh* LoadOBJ(std::string fileName);

	// Get texture
	inline GLuint GetTexture(GLuint index) const { return m_textures->at(index); }

	// User defined unfiforms will be set before drawing
	inline void SetUniforms(std::function<void(void)> onDrawCallBack) { m_OnDrawCallBack = onDrawCallBack; }

protected:
	// Loads VBO data into memory
	virtual void BufferAllData(GLboolean genBuffers = GL_TRUE);

	virtual void BufferVerticesData(GLboolean genBuffers);
	virtual void BufferNormalsData(GLboolean genBuffers){}
	virtual void BufferTexCoordData(GLboolean genBuffers);
	virtual void BufferColourData(GLboolean genBuffers);
	virtual void BufferIndicesData(GLboolean genBuffers);

	GLuint m_VAO;
	GLuint m_VBO[MGL_BUFFER_MAX];

	GLuint m_type;
	GLuint m_numIndices;
	GLuint m_numVertices;

	std::vector<glm::vec3>* m_vertices;
	std::vector<glm::vec3>* m_normals;
	std::vector<glm::vec2>* m_texCoords;
	std::vector<glm::vec4>* m_colours;

	std::vector<GLuint>* m_textures;
	std::vector<GLuint>* m_indices;

	std::function<void(void)> m_OnDrawCallBack;
};

class MGLCommonMeshes {
public:
	static MGLMesh* Quad() { return m_quad; }
	static MGLMesh* Triangle() { return m_triangle; }
	static MGLMesh* Cube() { return m_cube; }
	static MGLMesh* Sphere() { return m_sphere; }

	static void Init();
	static void Release();

protected:
	static MGLMesh* m_quad;
	static MGLMesh* m_triangle;
	static MGLMesh* m_cube;
	static MGLMesh* m_sphere;
};