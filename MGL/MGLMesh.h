#pragma once
#include "stdafx.h"

#include "MGLTexture.h"
#include "MGLFile.h"

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
	void SetNewColours(glm::vec4 colour, GLboolean buffer = GL_TRUE);

	// Set texture
	inline void AddTexture(GLuint tex) { m_textures->push_back(tex); }
	// Set Bump
	void RemoveTexture(GLuint tex);

	// Get texture
	inline GLuint GetTexture(GLuint index) const { return m_textures->at(index); }

	// User defined unfiforms will be set before drawing
	inline void SetUniforms(std::function<void(void)> onDrawCallBack) { m_OnDrawCallBack = onDrawCallBack; }

	MGLvecv2* GetTexCoords() const { return m_texCoords; }
	MGLvecv3* GetVertices() const { return m_vertices; }
	MGLvecv3* GetNormals() const { return m_normals; }
	MGLvecv4* GetColours() const { return m_colours; }

	MGLvecu* GetIndices() const { return m_indices; }
	GLuint GetNumVertices() const { return m_numVertices; }
	GLuint GetNumIndices() const { return m_numIndices; }
	GLuint GetType() const { return m_type; }

	void SetVertices(MGLvecv3* vec) { m_vertices = vec; }
	void SetTexCoords(MGLvecv2* vec) { m_texCoords = vec; }
	void SetColours(MGLvecv4* vec) { m_colours = vec; }
	void SetNormals(MGLvecv3* vec) { m_normals = vec; }

	void SetIndices(MGLvecu* vec) { m_indices = vec; }
	void SetNumVertices(GLuint num) { m_numVertices = num; }
	void SetNumIndices(GLuint num) { m_numIndices = num; }
	void SetType(GLenum type) { m_type = type; }

	// Loads VBO data into memory
	virtual void BufferAllData(GLboolean genBuffers = GL_TRUE);

	virtual void BufferVerticesData(GLboolean genBuffers);
	virtual void BufferNormalsData(GLboolean genBuffers){}
	virtual void BufferTexCoordData(GLboolean genBuffers);
	virtual void BufferColourData(GLboolean genBuffers);
	virtual void BufferIndicesData(GLboolean genBuffers);

protected:
	GLuint m_VAO;
	GLuint m_VBO[MGL_BUFFER_MAX];

	GLuint m_type;
	GLuint m_numIndices;
	GLuint m_numVertices;

	MGLvecv3* m_vertices;
	MGLvecv3* m_normals;
	MGLvecv2* m_texCoords;
	MGLvecv4* m_colours;

	MGLvecu* m_textures;
	MGLvecu* m_indices;

	std::function<void(void)> m_OnDrawCallBack;
};

class MGLCommonMeshes {
public:
	static MGLMesh* Quad() { return m_quad; }
	static MGLMesh* Triangle() { return m_triangle; }
	static MGLMesh* Cube() { return m_cube; }
	static MGLMesh* Sphere() { return m_sphere; }
	static MGLMesh* Cone() { return m_cone; }

	static void Init();
	static void Release();

protected:
	static MGLMesh* m_quad;
	static MGLMesh* m_triangle;
	static MGLMesh* m_cube;
	static MGLMesh* m_sphere;
	static MGLMesh* m_cone;
};