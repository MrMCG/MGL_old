#include "stdafx.h"
#include "MGLMeshGenerator.h"

#include "MGLMesh.h"

MGLMesh* MGLMeshGenerator::GenerateTriangle() {
	MGLMesh* mesh = new MGLMesh();
	GLuint numVert = 3;
	mesh->SetNumVertices(numVert);

	MGLvecv3* m_vertices = new MGLvecv3(numVert);
	MGLvecv2* m_texCoords = new MGLvecv2(numVert);
	MGLvecv3* m_normals = new MGLvecv3(numVert);
	MGLvecv4* m_colours = new MGLvecv4(numVert);

	m_vertices->at(0) = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertices->at(1) = glm::vec3(-1.0f, -1.0f, 0.0f);
	m_vertices->at(2) = glm::vec3(1.0f, -1.0f, 0.0f);

	m_texCoords->at(0) = glm::vec2(0.5f, 1.0f);
	m_texCoords->at(1) = glm::vec2(0.0f, 0.0f);
	m_texCoords->at(2) = glm::vec2(1.0f, 0.0f);

	for (GLuint i = 0; i < numVert; ++i) {
		m_normals->at(i) = glm::vec3(0.0f, 0.0f, 1.0f);
		m_colours->at(i) = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	mesh->SetVertices(m_vertices);
	mesh->SetTexCoords(m_texCoords);
	mesh->SetNormals(m_normals);
	mesh->SetColours(m_colours);

	mesh->BufferAllData();

	return mesh;
}

MGLMesh* MGLMeshGenerator::GenerateQuad() {
	MGLMesh* mesh = new MGLMesh();
	GLuint numVert = 4;
	GLuint numInd = 6;

	mesh->SetNumVertices(numVert);
	mesh->SetNumIndices(numInd);

	MGLvecv3* m_vertices = new MGLvecv3(numVert);
	MGLvecv2* m_texCoords = new MGLvecv2(numVert);
	MGLvecv3* m_normals = new MGLvecv3(numVert);
	MGLvecv4* m_colours = new MGLvecv4(numVert);

	std::vector<GLuint>* m_indices = new std::vector < GLuint >{ 2, 1, 3, 1, 0, 3 };

	m_vertices->at(0) = glm::vec3(-1.0f, -1.0f, 0.0f);
	m_vertices->at(1) = glm::vec3(-1.0f, 1.0f, 0.0f);
	m_vertices->at(2) = glm::vec3(1.0f, 1.0f, 0.0f);
	m_vertices->at(3) = glm::vec3(1.0f, -1.0f, 0.0f);

	m_texCoords->at(0) = glm::vec2(0.0f, 0.0f);
	m_texCoords->at(1) = glm::vec2(0.0f, 1.0f);
	m_texCoords->at(2) = glm::vec2(1.0f, 1.0f);
	m_texCoords->at(3) = glm::vec2(1.0f, 0.0f);

	for (GLuint i = 0; i < numVert; ++i) {
		m_normals->at(i) = glm::vec3(0.0f, 0.0f, 1.0f);
		m_colours->at(i) = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	mesh->SetVertices(m_vertices);
	mesh->SetTexCoords(m_texCoords);
	mesh->SetNormals(m_normals);
	mesh->SetColours(m_colours);
	mesh->SetIndices(m_indices);

	mesh->BufferAllData();

	return mesh;
}