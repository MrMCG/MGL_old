#include "stdafx.h"

#include "MGLMesh.h"
#include "MGLTexture.h"
#include "MGLFileLoader.h"

MGLMesh::MGLMesh() {

	for (auto i = 0; i < Max; ++i) {
		VBO[i] = 0;
	}

	glGenVertexArrays(1, &VAO);
}

MGLMesh::~MGLMesh() {
	if (vertices) {
		delete vertices;
		glDeleteBuffers(1, &VBO[Vertex]);
	}
	if (normals) {
		delete normals;
		glDeleteBuffers(1, &VBO[Normals]);
	}
	if (texCoords) {
		delete texCoords;
		glDeleteBuffers(1, &VBO[Textures]);
	}
	if (colours) {
		delete colours;
		glDeleteBuffers(1, &VBO[Colours]);
	}
	if (indices) {
		delete indices;
		glDeleteBuffers(1, &VBO[Indices]);
	}

	delete textures;
	glDeleteVertexArrays(1, &VAO);
}

void MGLMesh::Draw() {
	GLuint counter = 0;

	// load textures into texture units
	for (auto id : *textures) { 
		glActiveTexture(GL_TEXTURE0 + counter);
		glBindTexture(GL_TEXTURE_2D, id);
		counter++;
	}

	// run draw callback
	this->OnDrawCallBack();

	// bind VAO and draw
	glBindVertexArray(VAO);
	if (indices)
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, nullptr);
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

	if (vertices)
		BufferVerticesData(genBuffers, usage);

	if (normals)
		BufferNormalsData(genBuffers, usage);

	if (texCoords)
		BufferTexCoordData(genBuffers, usage);

	if (colours)
		BufferColourData(genBuffers, usage);

	if (indices) 
		BufferIndicesData(genBuffers, usage);
}

void MGLMesh::BufferVerticesData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[Vertex]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[Vertex]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec3), &vertices->at(0), usage);
	glVertexAttribPointer(Vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Vertex);

	glBindVertexArray(0);
}

void MGLMesh::BufferIndicesData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[Indices]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), &indices->at(0), usage);

	glBindVertexArray(0);
}


void MGLMesh::BufferTexCoordData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[Textures]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[Textures]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec2), &texCoords->at(0), usage);
	glVertexAttribPointer(Textures, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Textures);

	glBindVertexArray(0);
}


void MGLMesh::BufferColourData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[Colours]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[Colours]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec4), &colours->at(0), usage);
	glVertexAttribPointer(Colours, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Colours);

	glBindVertexArray(0);
}

void MGLMesh::BufferNormalsData(GLboolean genBuffers, const GLenum usage) {
	glBindVertexArray(VAO);

	if (genBuffers)
		glGenBuffers(1, &VBO[Normals]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[Normals]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec3), &normals->at(0), usage);
	glVertexAttribPointer(Normals, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Normals);

	glBindVertexArray(0);
}