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
	// Get texture
	GLuint GetTexture(GLuint index) const { return m_textures->at(index); }
	// User defined unfiforms will be set before drawing
	void SetUniforms(std::function<void(void)> onDrawCallBack) { m_OnDrawCallBack = onDrawCallBack; }

	MGLvecv3* GetVertices() const { return m_vertices; }
	MGLvecv2* GetTexCoords() const { return m_texCoords; }
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
	virtual void BufferAllData(GLboolean genBuffers = GL_TRUE, const GLenum usage = GL_STATIC_DRAW);

	virtual void BufferVerticesData(GLboolean genBuffers, const GLenum usage);
	virtual void BufferNormalsData(GLboolean genBuffers, const GLenum usage);
	virtual void BufferTexCoordData(GLboolean genBuffers, const GLenum usage);
	virtual void BufferColourData(GLboolean genBuffers, const GLenum usage);
	virtual void BufferIndicesData(GLboolean genBuffers, const GLenum usage);

	void GenerateNormals(){}; // TODO

protected:

	/****** Data ******/

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

class MGLCommonMeshes : public MGLSingleton<MGLCommonMeshes> {
	friend class MGLSingleton < MGLCommonMeshes > ;
public:

	// Returns a shared quad (DONT DELETE)
	MGLMesh* Quad() { return m_quad; }
	// Returns a shared triangle (DONT DELETE)
	MGLMesh* Triangle() { return m_triangle; }
	// Returns a shared cube (DONT DELETE)
	MGLMesh* Cube() { return m_cube; }
	// Returns a shared sphere (DONT DELETE)
	MGLMesh* Sphere() { return m_sphere; }
	// Returns a shared cone (DONT DELETE)
	MGLMesh* Cone() { return m_cone; }

protected:
	MGLCommonMeshes();
	~MGLCommonMeshes();

	/****** Data ******/

	MGLMesh* m_quad;
	MGLMesh* m_triangle;
	MGLMesh* m_cube;
	MGLMesh* m_sphere;
	MGLMesh* m_cone;
};

