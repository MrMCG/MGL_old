#include "stdafx.h"

#include "MGLMesh.h"
#include "MGLTexture.h"
#include "MGLFileLoader.h"

MGLMesh::MGLMesh() {

	for (int i = 0; i < MGL_BUFFER_MAX; ++i) {
		VBO[i] = 0;
	}

	glGenVertexArrays(1, &VAO);

	numIndices = 0;
	numVertices = 0;
	type = GL_TRIANGLES;

	vertices = nullptr;
	normals = nullptr;
	texCoords = nullptr;
	colours = nullptr;

	textures = new MGLvecu;
	indices = nullptr;

	OnDrawCallBack = [](){};
}

MGLMesh::~MGLMesh() {
	if (vertices) {
		delete vertices;
		glDeleteBuffers(1, &VBO[MGL_BUFFER_VERTEX]);
	}
	if (normals) {
		delete normals;
		glDeleteBuffers(1, &VBO[MGL_BUFFER_NORMALS]);
	}
	if (texCoords) {
		delete texCoords;
		glDeleteBuffers(1, &VBO[MGL_BUFFER_TEXTURES]);
	}
	if (colours) {
		delete colours;
		glDeleteBuffers(1, &VBO[MGL_BUFFER_COLOURS]);
	}
	if (indices) {
		delete indices;
		glDeleteBuffers(1, &VBO[MGL_BUFFER_INDICES]);
	}

	delete textures;
	glDeleteVertexArrays(1, &VAO);
}

void MGLMesh::Draw() {
	GLuint counter = 0;

	// load textures into texture units
	for (GLuint id : *textures) { 
		glActiveTexture(GL_TEXTURE0 + counter);
		glBindTexture(GL_TEXTURE_2D, id);
		counter++;
	}

	// run draw callback
	this->OnDrawCallBack();

	// bind VAO and draw
	glBindVertexArray(VAO);
	if (indices)
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(type, 0, numVertices);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void MGLMesh::SetNewColours(glm::vec4 colour, GLboolean buffer) {
	for (GLuint i = 0; i < numVertices; ++i) {
		colours->at(i) = colour;
	}

	if (buffer)
		BufferColourData(GL_FALSE, GL_STATIC_DRAW);
}

void MGLMesh::BufferAllData(const GLboolean genBuffers, const GLenum usage) {
	//buffer Vertices data
	if (vertices)
		BufferVerticesData(genBuffers, usage);

	//Buffer texture data
	if (normals)
		BufferNormalsData(genBuffers, usage);

	//Buffer texture data
	if (texCoords)
		BufferTexCoordData(genBuffers, usage);

	//buffer colour data
	if (colours)
		BufferColourData(genBuffers, usage);

	//Buffer indicies data
	if (indices) 
		BufferIndicesData(genBuffers, usage);
}

void MGLMesh::BufferVerticesData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[MGL_BUFFER_VERTEX]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[MGL_BUFFER_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec3), &vertices->at(0), usage);
	glVertexAttribPointer(MGL_BUFFER_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_VERTEX);

	glBindVertexArray(0);
}

void MGLMesh::BufferIndicesData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[MGL_BUFFER_INDICES]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[MGL_BUFFER_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), &indices->at(0), usage);

	glBindVertexArray(0);
}


void MGLMesh::BufferTexCoordData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[MGL_BUFFER_TEXTURES]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[MGL_BUFFER_TEXTURES]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec2), &texCoords->at(0), usage);
	glVertexAttribPointer(MGL_BUFFER_TEXTURES, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_TEXTURES);

	glBindVertexArray(0);
}


void MGLMesh::BufferColourData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[MGL_BUFFER_COLOURS]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[MGL_BUFFER_COLOURS]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec4), &colours->at(0), usage);
	glVertexAttribPointer(MGL_BUFFER_COLOURS, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_COLOURS);

	glBindVertexArray(0);
}

void MGLMesh::BufferNormalsData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[MGL_BUFFER_NORMALS]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[MGL_BUFFER_NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec3), &normals->at(0), usage);
	glVertexAttribPointer(MGL_BUFFER_NORMALS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_NORMALS);

	glBindVertexArray(0);
}

/***************************************/
/*********** MGLCommonMeshes ***********/
/***************************************/

MGLCommonMeshes::MGLCommonMeshes() {
	// load default texture
	GLuint m_defaultTex = MGLH_Tex->GetTexture("DEFAULT");

	m_cube = MGLI_FileLoaderMGL->Load(MGL_DEFAULT_CUBE);
	m_sphere = MGLI_FileLoaderMGL->Load(MGL_DEFAULT_SPHERE);
	m_cone = MGLI_FileLoaderMGL->Load(MGL_DEFAULT_CONE);

	// apply default textures
	m_cube->AddTexture(m_defaultTex);
	m_sphere->AddTexture(m_defaultTex);
	m_cone->AddTexture(m_defaultTex);
}

MGLCommonMeshes::~MGLCommonMeshes() {
	delete m_cube;
	delete m_sphere;
	delete m_cone;
}