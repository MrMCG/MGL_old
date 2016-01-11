#include "stdafx.h"
#include "MGLMesh.h"

#include <fstream>
#include <algorithm>

/*******************************/
/*********** MGLMesh ***********/
/*******************************/

MGLMesh::MGLMesh(MGLenum mgl_mesh_type) {

	for (int i = 0; i < MGL_BUFFER_MAX; ++i) {
		m_VBO[i] = 0;
	}

	glGenVertexArrays(1, &m_VAO);

	m_numIndices = 0;
	m_numVertices = 0;
	m_type = GL_TRIANGLES;

	m_vertices = nullptr;
	m_normals = nullptr;
	m_texCoords = nullptr;
	m_colours = nullptr;

	m_textures = new MGLvecu;
	m_indices = nullptr;

	m_OnDrawCallBack = [](){};

	switch (mgl_mesh_type) {
	case 0: break;
	case MGL_MESH_QUAD: GenerateQuad(); break;
	case MGL_MESH_TRIANGLE: GenerateTriangle(); break;
	default: break;
	}
}

MGLMesh::~MGLMesh() {
	if (m_vertices) {
		delete m_vertices;
		glDeleteBuffers(1, &m_VBO[MGL_BUFFER_VERTEX]);
	}
	if (m_normals) {
		delete m_normals;
		glDeleteBuffers(1, &m_VBO[MGL_BUFFER_NORMALS]);
	}
	if (m_texCoords) {
		delete m_texCoords;
		glDeleteBuffers(1, &m_VBO[MGL_BUFFER_TEXTURES]);
	}
	if (m_colours) {
		delete m_colours;
		glDeleteBuffers(1, &m_VBO[MGL_BUFFER_COLOURS]);
	}
	if (m_textures) {
		delete m_textures;
	}
	if (m_indices) {
		delete m_indices;
		glDeleteBuffers(1, &m_VBO[MGL_BUFFER_INDICES]);
	}

	glDeleteVertexArrays(1, &m_VAO);
}

void MGLMesh::Draw() {
	GLuint counter = 0;

	// load textures into texture units
	for (GLuint id : *m_textures) { 
		glActiveTexture(GL_TEXTURE0 + counter);
		glBindTexture(GL_TEXTURE_2D, id);
		counter++;
	}

	// run draw callback
	this->m_OnDrawCallBack();

	// bind VAO and draw
	glBindVertexArray(m_VAO);
	if (m_indices)
		glDrawElements(m_type, m_numIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(m_type, 0, m_numVertices);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void MGLMesh::GenerateTriangle() {
	m_numVertices = 3;

	m_vertices = new MGLvecv3(m_numVertices);
	m_texCoords = new MGLvecv2(m_numVertices);
	m_normals = new MGLvecv3(m_numVertices);
	m_colours = new MGLvecv4(m_numVertices);

	m_vertices->at(0) = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertices->at(1) = glm::vec3(-1.0f, -1.0f, 0.0f);
	m_vertices->at(2) = glm::vec3(1.0f, -1.0f, 0.0f);

	m_texCoords->at(0) = glm::vec2(0.5f, 1.0f);
	m_texCoords->at(1) = glm::vec2(0.0f, 0.0f);
	m_texCoords->at(2) = glm::vec2(1.0f, 0.0f);

	for (GLuint i = 0; i < m_numVertices; ++i) {
		m_normals->at(i) = glm::vec3(0.0f, 0.0f, 1.0f);
		m_colours->at(i) = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	BufferAllData();
}

void MGLMesh::GenerateQuad()	{
	m_numVertices = 4;
	m_numIndices = 6;

	m_vertices = new MGLvecv3(m_numVertices);
	m_texCoords = new MGLvecv2(m_numVertices);
	m_normals = new MGLvecv3(m_numVertices);
	m_colours = new MGLvecv4(m_numVertices);

	m_vertices->at(0) = glm::vec3(-1.0f, -1.0f, 0.0f);
	m_vertices->at(1) = glm::vec3(-1.0f, 1.0f, 0.0f);
	m_vertices->at(2) = glm::vec3(1.0f, 1.0f, 0.0f);
	m_vertices->at(3) = glm::vec3(1.0f, -1.0f, 0.0f);

	m_texCoords->at(0) = glm::vec2(0.0f, 0.0f);
	m_texCoords->at(1) = glm::vec2(0.0f, 1.0f);
	m_texCoords->at(2) = glm::vec2(1.0f, 1.0f);
	m_texCoords->at(3) = glm::vec2(1.0f, 0.0f);

	for (GLuint i = 0; i < m_numVertices; ++i) {
		m_normals->at(i) = glm::vec3(0.0f, 0.0f, 1.0f);
		m_colours->at(i) = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m_indices = new std::vector < GLuint >{ 2, 1, 3, 1, 0, 3 };

	BufferAllData();
}

void MGLMesh::SetNewColours(glm::vec4 colour, GLboolean buffer) {
	for (GLuint i = 0; i < m_numVertices; ++i) {
		m_colours->at(i) = colour;
	}

	if (buffer)
		BufferColourData(GL_FALSE, GL_STATIC_DRAW);
}

void MGLMesh::BufferAllData(const GLboolean genBuffers, const GLenum usage) {
	//buffer Vertices data
	if (m_vertices)
		BufferVerticesData(genBuffers, usage);

	//Buffer texture data
	if (m_normals)
		BufferNormalsData(genBuffers, usage);

	//Buffer texture data
	if (m_texCoords)
		BufferTexCoordData(genBuffers, usage);

	//buffer colour data
	if (m_colours)
		BufferColourData(genBuffers, usage);

	//Buffer indicies data
	if (m_indices) 
		BufferIndicesData(genBuffers, usage);
}

void MGLMesh::BufferVerticesData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_VERTEX]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec3), &m_vertices->at(0), usage);
	glVertexAttribPointer(MGL_BUFFER_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_VERTEX);

	glBindVertexArray(0);
}

void MGLMesh::BufferIndicesData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_INDICES]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[MGL_BUFFER_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices*sizeof(GLuint), &m_indices->at(0), usage);

	glBindVertexArray(0);
}


void MGLMesh::BufferTexCoordData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_TEXTURES]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_TEXTURES]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec2), &m_texCoords->at(0), usage);
	glVertexAttribPointer(MGL_BUFFER_TEXTURES, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_TEXTURES);

	glBindVertexArray(0);
}


void MGLMesh::BufferColourData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_COLOURS]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_COLOURS]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec4), &m_colours->at(0), usage);
	glVertexAttribPointer(MGL_BUFFER_COLOURS, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_COLOURS);

	glBindVertexArray(0);
}

void MGLMesh::BufferNormalsData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_NORMALS]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec3), &m_normals->at(0), usage);
	glVertexAttribPointer(MGL_BUFFER_NORMALS, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_NORMALS);

	glBindVertexArray(0);
}

/***************************************/
/*********** MGLCommonMeshes ***********/
/***************************************/

MGLCommonMeshes::MGLCommonMeshes() {
	// init dependencies if not already initialised
	MGLFileHandle->Init();
	MGLTexHandle->Init();

	// load default texture
	GLuint m_defaultTex = MGLTexHandle->GetTexture("DEFAULT");

	// create meshes
	m_quad = new MGLMesh(MGL_MESH_QUAD);
	m_triangle = new MGLMesh(MGL_MESH_TRIANGLE);
	m_cube = MGLFileHandle->LoadMGL(MGL_DEFAULT_CUBE);
	m_sphere = MGLFileHandle->LoadMGL(MGL_DEFAULT_SPHERE);
	m_cone = MGLFileHandle->LoadMGL(MGL_DEFAULT_CONE);

	// apply default textures
	m_quad->AddTexture(m_defaultTex);
	m_cube->AddTexture(m_defaultTex);
	m_triangle->AddTexture(m_defaultTex);
	m_sphere->AddTexture(m_defaultTex);
	m_cone->AddTexture(m_defaultTex);
}

MGLCommonMeshes::~MGLCommonMeshes() {
	delete m_quad;
	delete m_cube;
	delete m_triangle;
	delete m_sphere;
	delete m_cone;
}