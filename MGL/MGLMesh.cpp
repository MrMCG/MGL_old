#include "stdafx.h"
#include "MGLMesh.h"

#include <fstream>

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

	m_textures = new std::vector < GLuint > ;
	m_indices = nullptr;

	m_OnDrawCallBack = [](){};

	switch (mgl_mesh_type) {
	case 0: break;
	case MGL_MESH_QUAD: GenerateQuad(); break;
	case MGL_MESH_TRIANGLE: GenerateTriangle(); break;
	case MGL_MESH_CUBE: LoadOBJ(MGL_DEFAULT_CUBE); break;
	case MGL_MESH_SPHERE: LoadOBJ(MGL_DEFAULT_SPHERE); break;
	default: break;
	}
}

MGLMesh::~MGLMesh() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(MGL_BUFFER_MAX, m_VBO);

	if (m_vertices) 
		delete[] m_vertices;
	if (m_normals)
		delete[] m_normals;
	if (m_texCoords)
		delete[] m_texCoords;
	if (m_colours)
		delete[] m_colours;

	if (m_textures)
		delete m_textures;
	if (m_indices)
		delete m_indices;
}

void MGLMesh::Draw() {
	GLuint counter = 0;
	for (GLuint id : *m_textures) {
		glActiveTexture(GL_TEXTURE0 + counter);
		glBindTexture(GL_TEXTURE_2D, id);
		counter++;
	}

	this->m_OnDrawCallBack();

	glBindVertexArray(m_VAO);
	if (m_indices)
		glDrawElements(m_type, m_numIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(m_type, 0, m_numVertices);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void MGLMesh::BufferAllData(GLboolean genBuffers) {
	//buffer Vertices data
	if (m_vertices)
		BufferVerticesData(genBuffers);

	//buffer colour data
	if (m_colours)
		BufferColourData(genBuffers);

	//Buffer texture data
	if (m_texCoords) 
		BufferTexCoordData(genBuffers);

	//Buffer indicies data
	if (m_indices) 
		BufferIndicesData(genBuffers);
}

void MGLMesh::BufferVerticesData(GLboolean genBuffers) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_VERTEX]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec3), &m_vertices->at(0), GL_STATIC_DRAW);
	glVertexAttribPointer(MGL_BUFFER_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_VERTEX);

	glBindVertexArray(0);
}

void MGLMesh::BufferIndicesData(GLboolean genBuffers) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_INDICES]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[MGL_BUFFER_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices*sizeof(GLuint), &m_indices->at(0), GL_STATIC_DRAW);

	glBindVertexArray(0);
}


void MGLMesh::BufferTexCoordData(GLboolean genBuffers) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_TEXTURE]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_TEXTURE]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec2), &m_texCoords->at(0), GL_STATIC_DRAW);
	glVertexAttribPointer(MGL_BUFFER_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_TEXTURE);

	glBindVertexArray(0);
}


void MGLMesh::BufferColourData(GLboolean genBuffers) {
	glBindVertexArray(m_VAO);

	if (genBuffers)
		glGenBuffers(1, &m_VBO[MGL_BUFFER_COLOUR]);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[MGL_BUFFER_COLOUR]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(glm::vec4), &m_colours->at(0), GL_STATIC_DRAW);
	glVertexAttribPointer(MGL_BUFFER_COLOUR, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(MGL_BUFFER_COLOUR);

	glBindVertexArray(0);
}

void MGLMesh::SetColours(glm::vec4 colour, GLboolean buffer) {
	for (GLuint i = 0; i < m_numVertices; ++i) {
		m_colours->at(i) = colour;
	}

	if (buffer)
		BufferColourData(GL_FALSE);
}

void MGLMesh::RemoveTexture(GLuint tex) {}

void MGLMesh::GenerateTriangle() {
	m_numVertices = 3;

	m_vertices = new std::vector<glm::vec3>[m_numVertices];
	m_colours = new std::vector<glm::vec4>[m_numVertices];
	m_texCoords = new std::vector<glm::vec2>[m_numVertices];

	m_vertices->push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	m_vertices->push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	m_vertices->push_back(glm::vec3(1.0f, -1.0f, 0.0f));

	m_texCoords->push_back(glm::vec2(0.5f, 0.0f));
	m_texCoords->push_back(glm::vec2(0.0f, 1.0f));
	m_texCoords->push_back(glm::vec2(1.0f, 1.0f));

	m_colours->push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_colours->push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	m_colours->push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	BufferAllData();
}

void MGLMesh::GenerateQuad()	{
	m_numVertices = 4;
	m_numIndices = 6;
	m_type = GL_TRIANGLE_STRIP;

	m_vertices = new std::vector<glm::vec3>[m_numVertices];
	m_colours = new std::vector<glm::vec4>[m_numVertices];
	m_texCoords = new std::vector<glm::vec2>[m_numVertices];

	m_vertices->push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	m_vertices->push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	m_vertices->push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	m_vertices->push_back(glm::vec3(-1.0f, -1.0f, 0.0f));

	m_texCoords->push_back(glm::vec2(1.0f, 1.0f));
	m_texCoords->push_back(glm::vec2(1.0f, 0.0f));
	m_texCoords->push_back(glm::vec2(0.0f, 1.0f));
	m_texCoords->push_back(glm::vec2(0.0f, 0.0f));

	m_colours->push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_colours->push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	m_colours->push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_colours->push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	m_indices = new std::vector < GLuint >{ 0, 1, 2, 1, 2, 3 };

	BufferAllData();
}

MGLMesh* MGLMesh::LoadOBJ(std::string fileName) {


	std::ifstream file(fileName);

	std::vector<glm::vec2> inputTexCoords;
	std::vector<glm::vec3> inputVertices;
	std::vector<glm::vec3> inputNormals;
	
	std::vector<GLuint> vertexIndices;
	std::vector<GLuint> texIndices;
	std::vector<GLuint> normIndices;

	while (!file.eof()) {
		std::string token;
		file >> token;

		if (token == "v") {
			glm::vec3 vec;
			file >> vec.x; file >> vec.y; file >> vec.z;
			inputVertices.push_back(vec);
		}
		else if (token == "vn") {
			glm::vec3 vec;
			file >> vec.x; file >> vec.y; file >> vec.z;
			inputNormals.push_back(vec);
		}
		else if (token == "vt") {
			glm::vec2 vec;
			file >> vec.x; file >> vec.y;
			inputTexCoords.push_back(vec);
		}
		else if (token == "f") {
			std::string line;
			std::getline(file, line);

			for (GLuint i = 0; i < line.length(); ++i) {
				if (line[i] == '/')
					line[i] = ' ';
			}

			GLuint index = 0;
			GLuint data = 0;

			std::stringstream stream(line);
			while (stream >> data) {
				switch (index) {
				case 0: vertexIndices.push_back(data); break;
				case 1: texIndices.push_back(data); break;
				case 2: normIndices.push_back(data); break;
				}

				index++;
				index %= 3;
			}

		}
		

	}

	MGLMesh* mesh = new MGLMesh();

	mesh->m_numVertices = vertexIndices.size();
	mesh->m_vertices = new std::vector<glm::vec3>[mesh->m_numVertices];
	for (GLuint ind : vertexIndices) {
		mesh->m_vertices->push_back(inputVertices[ind-1]);
	}

	mesh->m_texCoords = new std::vector<glm::vec2>[mesh->m_numVertices];
	for (GLuint ind : texIndices) {
		mesh->m_texCoords->push_back(inputTexCoords[ind-1]);
	}

	mesh->m_colours = new std::vector<glm::vec4>[mesh->m_numVertices];
	for (GLuint i = 0; i < mesh->m_numVertices; ++i) {
		mesh->m_colours->push_back(MGL::WHITE);
	}

	mesh->BufferAllData();

	return mesh;
}

/***************************************/
/*********** MGLCommonMeshes ***********/
/***************************************/

MGLMesh* MGLCommonMeshes::m_cube = nullptr;
MGLMesh* MGLCommonMeshes::m_quad = nullptr;
MGLMesh* MGLCommonMeshes::m_triangle = nullptr;
MGLMesh* MGLCommonMeshes::m_sphere = nullptr;

void MGLCommonMeshes::Init() {
	if (!m_quad) {
		GLuint m_defaultTex = MGLTexHandle->GetTexture("DEFAULT");

		m_quad = new MGLMesh(MGL_MESH_QUAD);
		m_triangle = new MGLMesh(MGL_MESH_TRIANGLE);
		m_cube = MGLMesh::LoadOBJ(MGL_DEFAULT_CUBE);
		m_sphere = MGLMesh::LoadOBJ(MGL_DEFAULT_SPHERE);

		m_quad->AddTexture(m_defaultTex);
		m_cube->AddTexture(m_defaultTex);
		m_triangle->AddTexture(m_defaultTex);
		m_sphere->AddTexture(m_defaultTex);
	}
}

void MGLCommonMeshes::Release() {
	if (m_quad) {
		delete m_quad;
		delete m_cube;
		delete m_triangle;
		delete m_sphere;
	}
}