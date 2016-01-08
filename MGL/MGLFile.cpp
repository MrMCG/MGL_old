#include "stdafx.h"
#include "MGLFile.h"

#include "MGLMesh.h"

#include <fstream>
#include <algorithm>

void MGLFile::ConvertOBJToMGL(std::string fileName) {
	MGLMesh* mesh = LoadOBJ(fileName);

	std::stringstream stream = std::stringstream{};

	stream << mesh->GetType() << " ";
	stream << mesh->GetNumVertices() << " ";
	stream << mesh->GetNumIndices() << " ";
	stream << mesh->GetVertices()->size() << " ";
	stream << mesh->GetTexCoords()->size() << " ";
	stream << mesh->GetNormals()->size() << " ";
	stream << mesh->GetColours()->size() << " ";

	for (glm::vec3 const& vec : *mesh->GetVertices()) {
		stream << vec.x << " ";
		stream << vec.y << " ";
		stream << vec.z << " ";
	}

	for (glm::vec2 const& vec : *mesh->GetTexCoords()) {
		stream << vec.x << " ";
		stream << vec.y << " ";
	}

	for (glm::vec3 const& vec : *mesh->GetNormals()) {
		stream << vec.x << " ";
		stream << vec.y << " ";
		stream << vec.z << " ";
	}

	for (glm::vec4 const& vec : *mesh->GetColours()) {
		stream << vec.x << " ";
		stream << vec.y << " ";
		stream << vec.z << " ";
		stream << vec.w << " ";
	}

	for (GLuint const& ind : *mesh->GetIndices()) {
		stream << ind << " ";
	}

	std::ofstream file(fileName + ".mgl");
	file << stream.rdbuf();
	file.close();
}

MGLMesh* MGLFile::LoadMGL(std::string fileName, GLboolean buffer) {
	std::stringstream* stream = LoadFileToSS(fileName);

	GLuint numVertices, numIndices, type;
	GLuint vertSize, texSize, normalSize, colourSize;
	
	*stream >> type;
	*stream >> numVertices;
	*stream >> numIndices;
	*stream >> vertSize;
	*stream >> texSize;
	*stream >> normalSize;
	*stream >> colourSize;

	MGLvecu* indices = new MGLvecu;
	MGLvecv3* vertices = new MGLvecv3[vertSize];
	MGLvecv2* texCoords = new MGLvecv2[texSize];
	MGLvecv3* normals = new MGLvecv3[normalSize];
	MGLvecv4* colours = new MGLvecv4[colourSize];

	for (GLuint i = 0; i < vertSize; ++i) {
		glm::vec3 vec;
		*stream >> vec.x >> vec.y >> vec.z;
		vertices->push_back(vec);
	}

	for (GLuint i = 0; i < texSize; ++i) {
		glm::vec2 vec;
		*stream >> vec.x >> vec.y;
		texCoords->push_back(vec);
	}

	for (GLuint i = 0; i < normalSize; ++i) {
		glm::vec3 vec;
		*stream >> vec.x >> vec.y >> vec.z;
		normals->push_back(vec);
	}

	for (GLuint i = 0; i < colourSize; ++i) {
		glm::vec4 vec;
		*stream >> vec.x >> vec.y >> vec.z >> vec.w;
		colours->push_back(vec);
	}

	for (GLuint i = 0; i < numIndices; ++i) {
		GLuint temp;
		*stream >> temp;
		indices->push_back(temp);
	}

	MGLMesh* mesh = new MGLMesh();

	if (indices->size() != 0)
		mesh->SetIndices(indices);
	if (colours->size() != 0)
		mesh->SetColours(colours);
	if (normals->size() != 0)
		mesh->SetNormals(normals);
	if (texCoords->size() != 0)
		mesh->SetTexCoords(texCoords);

	mesh->SetVertices(vertices);
	mesh->SetNumIndices(numIndices);
	mesh->SetNumVertices(numVertices);
	mesh->SetType(type);

	if (buffer)
		mesh->BufferAllData();

	return mesh;
}

MGLMesh* MGLFile::LoadOBJ(std::string fileName, GLboolean buffer) {

	MGLObjFileData* obj = new MGLObjFileData();

	std::stringstream* stream = LoadFileToSS(fileName);

	std::string token;

	while (!stream->eof()) {
		token = "";
		*stream >> token;

		if (token == "#") {
			std::string line;
			std::getline(*stream, line);
			continue;
		}
		else if (token == "0") {
			continue;
		}
		else if (token == "v") {
			glm::vec3 vec;
			*stream >> (GLfloat)vec.x >> (GLfloat)vec.y >> (GLfloat)vec.z;
			obj->inputVertices.push_back(vec);
		}
		else if (token == "vn") {
			glm::vec3 vec;
			*stream >> (GLfloat)vec.x >> (GLfloat)vec.y >> (GLfloat)vec.z;
			obj->inputNormals.push_back(vec);
		}
		else if (token == "vt") {
			glm::vec2 vec;
			*stream >> (GLfloat)vec.x >> (GLfloat)vec.y;
			obj->inputTexCoords.push_back(vec);
		}
		else if (token == "f") {
			std::string line;
			std::getline(*stream, line);

			HandleOBJFace(line, obj);
		}
	}

	std::sort(obj->objVertexList.begin(), obj->objVertexList.end());

	MGLMesh* mesh = CreateMesh(obj);
	delete obj;

	if (buffer)
		mesh->BufferAllData();

	return mesh;
}


std::stringstream* MGLFile::LoadFileToSS(std::string fileName) {
	std::ifstream file(fileName);

	if (!file) {
		return nullptr;
	}

	std::stringstream* stream = new std::stringstream{};
	*stream << file.rdbuf();
	file.close();

	return stream;
}

void MGLFile::HandleOBJFace(std::string line, MGLObjFileData* obj) {
	std::vector<GLuint> indicies;
	GLuint data = 0;
	GLuint spaceCounter = 0;
	GLuint slashCounter = 0;

	GLuint idCounter = 0;
	GLuint idOfVert = -1;
	GLboolean vertFound = GL_FALSE;

	GLboolean skipTex = GL_FALSE;
	GLboolean skipNorm = GL_FALSE;
	GLboolean lastCharSpace = GL_FALSE;
	GLboolean isPolygon = GL_FALSE;

	if (line.find("//") != std::string::npos) {
		skipTex = GL_TRUE;
	}

	for (char& c : line) {

		if (c == ' ') {
			lastCharSpace = GL_TRUE;
		}
		else if (c == '/') {
			c = ' ';
			slashCounter++;
		}
		else if (lastCharSpace) {
			spaceCounter++;
			lastCharSpace = GL_FALSE;
		}

	}

	if (slashCounter / spaceCounter == 1 && !skipTex) {
		skipNorm = GL_TRUE;
	}

	if (spaceCounter == 4)
		isPolygon = GL_TRUE;

	std::stringstream stream(line);
	while (stream >> data) {
		indicies.push_back(data);
	}


	if (skipTex) { // v1//vt1 v2//vt2 v3//vt3
		for (GLuint i = 0; i < (spaceCounter * 2) - 1; i += 2) {
			obj->vertexIndices.push_back(indicies[i]);
			obj->normalIndices.push_back(indicies[i + 1]);

			MGLObjVertData vert = MGLObjVertData();
			vert.vertex = indicies[i];
			vert.normals = indicies[i + 1];

			obj->finalIndices.push_back(AddOBJVertToList(obj, vert));

			if (i == 4 && isPolygon) {
				i -= 4;
				isPolygon = GL_FALSE;
			}
		}
	}
	else if (skipNorm) { // v1/vt1 v2/vt2 v3/vt3
		for (GLuint i = 0; i < (spaceCounter * 2) - 1; i += 2) {
			obj->vertexIndices.push_back(indicies[i]);
			obj->texIndices.push_back(indicies[i + 1]);

			MGLObjVertData vert = MGLObjVertData();
			vert.vertex = indicies[i];
			vert.texture = indicies[i + 1];

			obj->finalIndices.push_back(AddOBJVertToList(obj, vert));

			if (i == 4 && isPolygon) {
				i -= 4;
				isPolygon = GL_FALSE;
			}
		}
	}
	else { // v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
		for (GLuint i = 0; i < (spaceCounter * 3) - 1; i += 3) {
			obj->vertexIndices.push_back(indicies[i]);
			obj->texIndices.push_back(indicies[i + 1]);
			obj->normalIndices.push_back(indicies[i + 2]);

			MGLObjVertData vert = MGLObjVertData();
			vert.vertex = indicies[i];
			vert.texture = indicies[i + 1];
			vert.normals = indicies[i + 2];

			GLuint id = AddOBJVertToList(obj, vert);

			obj->finalIndices.push_back(id);

			if (i == 6 && isPolygon) {
				i -= 6;
				isPolygon = GL_FALSE;
			}
		}
	}
}

GLuint MGLFile::AddOBJVertToList(MGLObjFileData* obj, MGLObjVertData& vert) {
	
	GLboolean vertFound = GL_FALSE;
	GLuint idOfVert = 0;

	for (MGLObjVertData& v : obj->objVertexList) {
		if (v.Compare(vert)) {
			vertFound = GL_TRUE;
			idOfVert = v.id;
		}
	}

	if (!vertFound) {
		vert.id = ++obj->idCounter;
		obj->objVertexList.push_back(vert);
		idOfVert = vert.id;
	}

	return idOfVert;
}

MGLMesh* MGLFile::CreateMesh(MGLObjFileData* obj) {

	MGLMesh* mesh = new MGLMesh();

	GLuint numVertices = obj->objVertexList.size();
	MGLvecu* indices = new MGLvecu;
	MGLvecv3* vertices = new MGLvecv3;
	MGLvecv2* texCoords = new MGLvecv2;
	MGLvecv3* normals = new MGLvecv3;
	MGLvecv4* colours = new MGLvecv4;

	for (MGLObjVertData& data : obj->objVertexList) {
		vertices->push_back(obj->inputVertices.at(data.vertex - 1));
		if (data.texture)
			texCoords->push_back(obj->inputTexCoords.at(data.texture - 1));
		if (data.normals)
			normals->push_back(obj->inputNormals.at(data.normals - 1));
	}

	for (GLuint i = 0; i < numVertices; ++i) {
		colours->push_back(MGL::WHITE);
	}

	for (GLuint ind : obj->finalIndices) {
		indices->push_back(ind - 1);
	}

	if (indices->size() != 0)
		mesh->SetIndices(indices);
	if (colours->size() != 0)
		mesh->SetColours(colours);
	if (normals->size() != 0)
		mesh->SetNormals(normals);
	if (texCoords->size() != 0)
		mesh->SetTexCoords(texCoords);

	mesh->SetVertices(vertices);
	mesh->SetNumIndices(indices->size());
	mesh->SetNumVertices(numVertices);

	return mesh;
}