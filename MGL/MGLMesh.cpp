#include "stdafx.h"
#include "MGLMesh.h"

/*******************************/
/*********** MGLMesh ***********/
/*******************************/
MGLMesh::MGLMesh() {

	for (int i = 0; i < MGL_BUFFER_MAX; ++i) {
		m_VBO[i] = 0;
	}

	glGenVertexArrays(1, &m_VAO);

	m_numIndices = 0;
	m_numVertices = 0;
	m_type = GL_TRIANGLES;

	m_indices = nullptr;

	m_vertices = nullptr;
	m_colours = nullptr;
	m_texCoords = nullptr;
	m_normals = nullptr;
	m_tangents = nullptr;
}

MGLMesh::~MGLMesh() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(MGL_BUFFER_MAX, m_VBO);

	if (m_vertices)
		delete m_vertices;
	if (m_colours)
		delete m_colours;
	if (m_texCoords)
		delete m_texCoords;
	if (m_normals)
		delete m_normals;
	if (m_tangents)
		delete m_tangents;
}

void MGLMesh::Draw() {
	glBindVertexArray(m_VAO);
	if (m_indices)
		glDrawElements(m_type, m_numIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(m_type, 0, m_numVertices);
	glBindVertexArray(0);
}

void MGLMesh::BufferData() {
	glBindVertexArray(m_VAO);

	//Buffer vertex data
	glGenBuffers(1, &m_VBO[MGL_BUFFER_VERTEX]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec3), m_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(MGL_BUFFER_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_VERTEX);

	//buffer colour data
	if (m_colours)	{
		glGenBuffers(1, &m_VBO[MGL_BUFFER_COLOUR]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_COLOUR]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec4), m_colours, GL_STATIC_DRAW);
		glVertexAttribPointer(MGL_BUFFER_COLOUR, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(MGL_BUFFER_COLOUR);
	}

	//Buffer texture data
	if (m_texCoords) {
		glGenBuffers(1, &m_VBO[MGL_BUFFER_TEXTURE]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_TEXTURE]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec2), m_texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(MGL_BUFFER_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(MGL_BUFFER_TEXTURE);
	}

	//Buffer indicies data
	if (m_indices) {
		glGenBuffers(1, &m_VBO[MGL_BUFFER_INDICES]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[MGL_BUFFER_INDICES]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices*sizeof(GLuint), m_indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void MGLMesh::SetColours(glm::vec4 colour) {
	for (GLuint i = 0; i < m_numVertices; ++i) {
		m_colours[i] = colour;
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_COLOUR]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec4), m_colours, GL_STATIC_DRAW);
	glVertexAttribPointer(MGL_BUFFER_COLOUR, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_COLOUR);
}

void MGLMesh::GenerateTriangle() {
	m_numVertices = 3;

	m_vertices = new glm::vec3[3];
	m_colours = new glm::vec4[3];
	m_texCoords = new glm::vec2[3];

	m_vertices[0] = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertices[1] = glm::vec3(-1.0f, -1.0f, 0.0f);
	m_vertices[2] = glm::vec3(1.0f, -1.0f, 0.0f);

	m_texCoords[0] = glm::vec2(0.5f, 0.0f);
	m_texCoords[1] = glm::vec2(0.0f, 1.0f);
	m_texCoords[2] = glm::vec2(1.0f, 1.0f);

	m_colours[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_colours[1] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	m_colours[2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	BufferData();
}

void MGLMesh::GenerateQuad()	{
	m_numVertices = 4;
	m_numIndices = 6;
	m_type = GL_TRIANGLE_STRIP;

	m_vertices = new glm::vec3[4];
	m_colours = new glm::vec4[4];
	m_texCoords = new glm::vec2[4];

	m_vertices[0] = glm::vec3(1.0f, 1.0f, 0.0f);
	m_vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
	m_vertices[2] = glm::vec3(-1.0f, 1.0f, 0.0f);
	m_vertices[3] = glm::vec3(-1.0f, -1.0f, 0.0f);

	m_texCoords[0] = glm::vec2(1.0f, 1.0f);
	m_texCoords[1] = glm::vec2(1.0f, 0.0f);
	m_texCoords[2] = glm::vec2(0.0f, 1.0f);
	m_texCoords[3] = glm::vec2(0.0f, 0.0f);

	m_colours[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_colours[1] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	m_colours[2] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	m_colours[3] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	m_indices = new GLuint[6] {
		0, 1, 2,
		1, 2, 3
	};

	BufferData();
}

/***************************************/
/*********** MGLMeshTextured ***********/
/***************************************/
MGLMeshTextured::MGLMeshTextured() : MGLMesh() {
	m_tex = 0;
	m_bump = 0;
	m_OnDrawCallBack = [](){};
}

void MGLMeshTextured::Draw() {
	if (m_tex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_tex);
	}

	if (m_bump) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_bump);
	}
	this->m_OnDrawCallBack();
	MGLMesh::Draw();
	glBindTexture(GL_TEXTURE_2D, 0);
}