#include "stdafx.h"
#include "MGLFile.h"

#include "MGLMesh.h"

#include <fstream>
#include <sstream>

// most commonly used 
using std::stringstream;
using std::string;
using std::ifstream;

void MGLFile::ConvertOBJToMGL(std::string fileName, GLboolean saveColours) {
	try {
		MGLMesh* mesh = LoadOBJ(fileName);
		SaveMeshToMGL(mesh, fileName, saveColours);
	}
	catch (MGLException& e) {
		std::cerr << std::endl << e.what() << std::endl;
		return;
	}
}

MGLMesh* MGLFile::LoadMGL(std::string fileName, GLboolean bufferData) {
	
	ifstream file;

	std::size_t size = 0;
	try {

		// open file
		file.open(fileName, std::ios::in | std::ios::binary);

		MGLException_FileNotFound::IsSuccessful(file.is_open(), fileName);
		MGLException_File_FileType::IsSuccessful(fileName, ".mgl");

		file.seekg(0, std::ios_base::end);
		size = (size_t)file.tellg(); // get file size
		file.seekg(0, std::ios_base::beg);

		MGLException_SizeTooSmall::IsSuccessful(size, MGL_FILE_MINSIZE, fileName);
	}
	catch (MGLException& e) {
		std::cerr << std::endl << e.what() << std::endl;
		return MGLCommonMeshes::Cube();
	}

	// read file buffer into vector
	std::vector<GLfloat> buffer(size / sizeof(GLfloat));
	file.read((GLchar*)&buffer[0], size);

	GLuint numVertices, numIndices, type;
	GLuint vertSize, texSize, normalSize, colourSize;

	GLuint loc = 0;

	// build mesh from vector
	type = (GLuint)buffer[loc];
	numVertices = (GLuint)buffer[++loc];
	numIndices = (GLuint)buffer[++loc];
	vertSize = (GLuint)buffer[++loc];
	texSize = (GLuint)buffer[++loc];
	normalSize = (GLuint)buffer[++loc];
	colourSize = (GLuint)buffer[++loc];

	MGLMesh* mesh = new MGLMesh();

	MGLvecu* indices = new MGLvecu(numIndices);
	MGLvecv3* vertices = new MGLvecv3(vertSize);
	MGLvecv2* texCoords = new MGLvecv2(texSize);
	MGLvecv3* normals = new MGLvecv3(normalSize);
	MGLvecv4* colours = new MGLvecv4(colourSize);


	for (GLuint i = 0; i < vertSize && file.good(); ++i) {
		glm::vec3 vec;
		vec.x = buffer[++loc];
		vec.y = buffer[++loc];
		vec.z = buffer[++loc];
		vertices->at(i) = vec;
	}

	for (GLuint i = 0; i < texSize && file.good(); ++i) {
		glm::vec2 vec;
		vec.x = buffer[++loc];
		vec.y = buffer[++loc];
		texCoords->at(i) = vec;
	}

	for (GLuint i = 0; i < normalSize && file.good(); ++i) {
		glm::vec3 vec;
		vec.x = buffer[++loc];
		vec.y = buffer[++loc];
		vec.z = buffer[++loc];
		normals->at(i) = vec;
	}

	for (GLuint i = 0; i < colourSize && file.good(); ++i) {
		glm::vec4 vec;
		vec.x = buffer[++loc];
		vec.y = buffer[++loc];
		vec.z = buffer[++loc];
		vec.w = buffer[++loc];
		colours->at(i) = vec;
	}

	for (GLuint i = 0; i < numIndices && file.good(); ++i) {
		GLuint temp;
		temp = (GLuint)buffer[++loc];
		indices->at(i) = temp;
	}

	// correctly set mesh properties
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

	if (bufferData)
		mesh->BufferAllData();

	return mesh;
	
}

void MGLFile::SaveMeshToMGL(MGLMesh* mesh, std::string fileName, GLboolean saveColours) {
	// open file
	std::ofstream out; 

	try {
		out.open(fileName + ".mgl", std::ios::binary);
		MGLException_FileNotFound::IsSuccessful(out.is_open(), fileName);
	}
	catch (MGLException& e) {
		std::cerr << std::endl << e.what() << std::endl;
		return;
	}

	GLfloat val = 0;
	// type
	val = (GLfloat)mesh->GetType();
	out.write((GLchar*)&val, sizeof(GLfloat));
	// numVertices
	val = (GLfloat)mesh->GetNumVertices();
	out.write((GLchar*)&val, sizeof(GLfloat));
	// numIndices
	val = (GLfloat)mesh->GetNumIndices();
	out.write((GLchar*)&val, sizeof(GLfloat));
	// size of vertices vector
	val = (GLfloat)mesh->GetVertices()->size();
	out.write((GLchar*)&val, sizeof(GLfloat));
	// size of texCoord vector
	val = (GLfloat)mesh->GetTexCoords()->size();
	out.write((GLchar*)&val, sizeof(GLfloat));
	// size of normals vector
	val = (GLfloat)mesh->GetNormals()->size();
	out.write((GLchar*)&val, sizeof(GLfloat));

	// size of colours vector
	val = 0;
	if (saveColours)
		val = (GLfloat)mesh->GetColours()->size();

	out.write((GLchar*)&val, sizeof(GLfloat));

	// write mesh to file
	for (glm::vec3 const& vec : *mesh->GetVertices()) {
		out.write((GLchar*)&vec.x, sizeof(GLfloat));
		out.write((GLchar*)&vec.y, sizeof(GLfloat));
		out.write((GLchar*)&vec.z, sizeof(GLfloat));
	}

	for (glm::vec2 const& vec : *mesh->GetTexCoords()) {
		out.write((GLchar*)&vec.x, sizeof(GLfloat));
		out.write((GLchar*)&vec.y, sizeof(GLfloat));
	}

	for (glm::vec3 const& vec : *mesh->GetNormals()) {
		out.write((GLchar*)&vec.x, sizeof(GLfloat));
		out.write((GLchar*)&vec.y, sizeof(GLfloat));
		out.write((GLchar*)&vec.z, sizeof(GLfloat));
	}

	if (saveColours) {
		for (glm::vec4 const& vec : *mesh->GetColours()) {
			out.write((GLchar*)&vec.x, sizeof(GLfloat));
			out.write((GLchar*)&vec.y, sizeof(GLfloat));
			out.write((GLchar*)&vec.z, sizeof(GLfloat));
			out.write((GLchar*)&vec.w, sizeof(GLfloat));
		}
	}

	for (GLuint const& ind : *mesh->GetIndices()) {
		GLfloat val = (GLfloat)ind;
		out.write((GLchar*)&val, sizeof(GLfloat));
	}
	
	out.close();
}

MGLMesh* MGLFile::LoadOBJ(std::string fileName, GLboolean bufferData) {

	MGLObjFileData* obj = new MGLObjFileData();
	// init data with *blanks* for defaults
	obj->inputVertices.push_back(glm::vec3(0, 0, 0));
	obj->inputNormals.push_back(glm::vec3(0, 0, 0));
	obj->inputTexCoords.push_back(glm::vec2(0, 0));

	// load file into stringstream
	stringstream* stream = LoadFileToSS(fileName);

	try {
		MGLException_NullError::IsSuccessful(stream);
		MGLException_File_FileType::IsSuccessful(fileName, ".obj");
	}
	catch (MGLException& e) {
		std::cerr << std::endl << e.what() << " : STRINGSTREAM LoadOBJ " << std::endl;
		return MGLCommonMeshes::Cube();
	}

	string token;

	// parse file from line start and build vertex data
	while (!stream->eof()) {
		token = "";
		*stream >> token;

		if (token == "#") { // a comment
			string line;
			std::getline(*stream, line);
			continue;
		}
		else if (token == "0") { // empty
			continue;
		}
		else if (token == "v") { // vertex
			glm::vec3 vec;
			*stream >> (GLfloat)vec.x >> (GLfloat)vec.y >> (GLfloat)vec.z;
			obj->inputVertices.push_back(vec);
		}
		else if (token == "vn") { // normals
			glm::vec3 vec;
			*stream >> (GLfloat)vec.x >> (GLfloat)vec.y >> (GLfloat)vec.z;
			obj->inputNormals.push_back(vec);
		}
		else if (token == "vt") { // texture coords
			glm::vec2 vec;
			*stream >> (GLfloat)vec.x >> (GLfloat)vec.y;
			obj->inputTexCoords.push_back(vec);
		}
		else if (token == "f") { // face
			std::string line;
			std::getline(*stream, line);

			HandleOBJFace(line, obj);
		}
	}

	// sort list so indices-1 (1 start) = index of (0 start)
	//std::sort(obj->objVertexList.begin(), obj->objVertexList.end());

	MGLMesh* mesh = CreateMesh(obj);
	delete obj;

	if (bufferData)
		mesh->BufferAllData();

	return mesh;
}


std::stringstream* MGLFile::LoadFileToSS(std::string fileName) {
	ifstream file(fileName);

	try {
		MGLException_FileNotFound::IsSuccessful(file.is_open(), fileName);
	}
	catch (MGLException& e) {
		std::cerr << std::endl << e.what() << std::endl;
		return nullptr;
	}

	stringstream* stream = new stringstream{};
	*stream << file.rdbuf();
	file.close();

	return stream;
}

void MGLFile::HandleOBJFace(std::string line, MGLObjFileData* obj) {
	MGLvecu indicies;
	GLuint data = 0;
	GLuint spaceCounter = 0;
	GLuint slashCounter = 0;

	GLboolean vertFound = GL_FALSE;
	GLboolean skipTex = GL_FALSE;
	GLboolean skipNorm = GL_FALSE;
	GLboolean lastCharSpace = GL_FALSE;
	GLboolean isPolygon = GL_FALSE;

	if (line.find("//") != std::string::npos) {
		skipTex = GL_TRUE;
	}

	for (GLchar& c : line) {

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

	stringstream stream(line);
	while (stream >> data) {
		indicies.push_back(data);
	}

	GLboolean skipvertices = GL_FALSE;

	if (skipTex || skipNorm) {
		for (GLuint i = 0; i < (spaceCounter * 2) - 1; i += 2) {
			MGLObjVertData vert = MGLObjVertData();

			obj->vertexIndices.push_back(indicies[i]);
			if (skipTex) {
				obj->texIndices.push_back(0);
				vert.texture = 0;
			}
			else {
				obj->texIndices.push_back(indicies[i + 1]);
				vert.texture = indicies[i + 1];
			}
			if (skipNorm) {
				obj->texIndices.push_back(0);
				vert.normals = 0;
			}
			else {
				obj->normalIndices.push_back(indicies[i + 1]);
				vert.normals = indicies[i + 1];
			}

			vert.vertex = indicies[i];

			GLuint id = AddOBJVertToList(obj, vert);

			obj->finalIndices.push_back(id);

			if (skipvertices) {
				i += 4;
				skipvertices = GL_FALSE;
			}

			if (i == 4 && isPolygon) {
				i -= 4;
				isPolygon = GL_FALSE;
				skipvertices = GL_TRUE;
			}
		}
	}
	else {
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

			if (skipvertices) {
				i += 6;
				skipvertices = GL_FALSE;
			}

			if (i == 6 && isPolygon) {
				i -= 6;
				isPolygon = GL_FALSE;
				skipvertices = GL_TRUE;
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
		vertices->push_back(obj->inputVertices.at(data.vertex));
		texCoords->push_back(obj->inputTexCoords.at(data.texture));
		normals->push_back(obj->inputNormals.at(data.normals));
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
	mesh->SetNumVertices(numVertices);
	mesh->SetNumIndices(indices->size());

	return mesh;
}