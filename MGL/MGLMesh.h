#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

#define MGL_BUFFER_VERTEX 0
#define MGL_BUFFER_TEXTURES 1
#define MGL_BUFFER_NORMALS 2
#define MGL_BUFFER_COLOURS 3
#define MGL_BUFFER_INDICES 4
#define MGL_BUFFER_MAX 5

#define MGLH_ComMesh MGLCommonMeshes::Instance()
#define MGLH_Tex MGLTexture::Instance()

class MGLMesh {
public:
	MGLMesh();
	virtual ~MGLMesh();

	// Draws the mesh
	virtual void Draw();

	// Sets vertices amount of colours to specified colour
	void SetNewColours(glm::vec4 colour, GLboolean buffer = GL_TRUE);
	// Set texture
	void AddTexture(GLuint tex) { textures->push_back(tex); }
	// Get texture
	GLuint GetTexture(GLuint index) const { return textures->at(index); }
	// User defined unfiforms will be set before drawing
	void SetUniforms(std::function<void(void)> onDrawCallBack) { OnDrawCallBack = onDrawCallBack; }

	MGLvecv3* GetVertices() const { return vertices; }
	MGLvecv2* GetTexCoords() const { return texCoords; }
	MGLvecv3* GetNormals() const { return normals; }
	MGLvecv4* GetColours() const { return colours; }

	MGLvecu* GetIndices() const { return indices; }
	GLuint GetNumVertices() const { return numVertices; }
	GLuint GetNumIndices() const { return numIndices; }
	GLuint GetType() const { return type; }

	void SetVertices(MGLvecv3* vec) { vertices = vec; }
	void SetTexCoords(MGLvecv2* vec) { texCoords = vec; }
	void SetColours(MGLvecv4* vec) { colours = vec; }
	void SetNormals(MGLvecv3* vec) { normals = vec; }

	void SetIndices(MGLvecu* vec) { indices = vec; }
	void SetNumVertices(GLuint num) { numVertices = num; }
	void SetNumIndices(GLuint num) { numIndices = num; }
	void SetType(GLenum typ) { type = typ; }

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

	GLuint VAO;
	GLuint VBO[MGL_BUFFER_MAX];

	GLuint type;
	GLuint numIndices;
	GLuint numVertices;

	MGLvecv3* vertices;
	MGLvecv3* normals;
	MGLvecv2* texCoords;
	MGLvecv4* colours;

	MGLvecu* textures;
	MGLvecu* indices;

	std::function<void(void)> OnDrawCallBack;
};

class MGLCommonMeshes : public MGLSingleton<MGLCommonMeshes> {
	friend class MGLSingleton < MGLCommonMeshes > ;
public:

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

	MGLMesh* m_cube;
	MGLMesh* m_sphere;
	MGLMesh* m_cone;
};

