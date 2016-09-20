#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

/*
 * TODO: refactor textures into materials
 * TODO: add tangents data
 * TODO: add ability to generate normals + tangents
 */

class MGLMesh {
public:
	MGLMesh();
	virtual ~MGLMesh();

	// Draws the mesh
	virtual void Draw();

	void SetNewColours(const glm::vec4 colour, const GLboolean buffer = GL_TRUE);
	void AddTexture(const GLuint tex) const { textures->push_back(tex); }
	GLuint GetTexture(const GLuint index) const { return textures->at(index); }
	void SetDrawCallback(std::function<void(void)> onDrawCallBack) { OnDrawCallBack = onDrawCallBack; }

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

	virtual void BufferAllData(GLboolean genBuffers = GL_TRUE, const GLenum usage = GL_STATIC_DRAW);

	virtual void BufferVerticesData(GLboolean genBuffers, const GLenum usage);
	virtual void BufferNormalsData(GLboolean genBuffers, const GLenum usage);
	virtual void BufferTexCoordData(GLboolean genBuffers, const GLenum usage);
	virtual void BufferColourData(GLboolean genBuffers, const GLenum usage);
	virtual void BufferIndicesData(GLboolean genBuffers, const GLenum usage);

	void GenerateNormals(){}; // TODO

	enum BufferIndexes {Vertex, Textures, Normals, Colours, Indices, Max};

	MGLMesh(const MGLMesh& other) = delete;
	MGLMesh(const MGLMesh&& other) = delete;
	MGLMesh& operator=(const MGLMesh& other) = delete;
	MGLMesh& operator=(const MGLMesh&& other) = delete;

protected:

	/****** Data ******/

	GLuint VAO = 0;
	GLuint VBO[Max];

	GLuint type = GL_TRIANGLES;
	GLuint numIndices = 0;
	GLuint numVertices = 0;

	MGLvecv3* vertices = nullptr;
	MGLvecv3* normals = nullptr;
	MGLvecv2* texCoords = nullptr;
	MGLvecv4* colours = nullptr;

	MGLvecu* textures = new MGLvecu();
	MGLvecu* indices = nullptr;

	std::function<void(void)> OnDrawCallBack = [](){};
};

