#include "stdafx.h"
#include "MGLFile.h"

#include "MGLMesh.h"

#include <fstream>
#include <sstream>
#include <algorithm>

// most commonly used 
using std::stringstream;
using std::string;
using std::ifstream;

void MGLFile::ConvertOBJToMGL(const std::string& fileName, const std::string& title, const GLboolean saveColours) {
	MGLMesh* mesh = LoadOBJ(fileName);

	// make sure mesh is not a triangle (error mesh)
	try {
		MGLException_IsLessThan::IsSuccessful(mesh->GetNumVertices(), (GLuint)4);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << ": OBJ LOADING ERROR FROM " << fileName << std::endl;
		delete mesh;
		return;
	}

	SaveMeshToMGL(mesh, title, saveColours);
	delete mesh;
}

MGLMesh* MGLFile::LoadMGL(std::string fileName, GLboolean bufferData) {
	
	ifstream file;

	std::size_t size = 0;
	try {
		// open file
		file.open(fileName, std::ios::in | std::ios::binary);

		MGLException_FileError::IsSuccessful(file.is_open(), fileName);
		MGLException_File_FileType::IsSuccessful(fileName, ".mgl");

		file.seekg(0, std::ios_base::end);
		size = (std::size_t)file.tellg(); // get file size
		file.seekg(0, std::ios_base::beg);

		MGLException_FileSTooSmall::IsSuccessful(fileName, file, MGL_FILE_MINSIZE);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << std::endl;
		return new MGLMesh(MGL_MESH_TRIANGLE);
	}

	// read file buffer into vector
	std::vector<GLfloat>* buffer = new std::vector<GLfloat>(size / sizeof(GLfloat));
	file.read((GLchar*)&buffer->at(0), size);
	file.close();

	try {
		MGLException_IsNotEqual::IsSuccessful(DetermineFileSize(
			(GLuint)buffer->at(2), (GLuint)buffer->at(3), (GLint)buffer->at(4)), size);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << ": FILE SIZE ERROR " << fileName << std::endl;
		return new MGLMesh(MGL_MESH_TRIANGLE);
	}

	MGLMesh* mesh = LoadMesh(buffer, bufferData);

	delete buffer;

	return mesh;	
}

MGLMesh* MGLFile::LoadMesh(const MGLvecf* buffer, const GLboolean bufferData) {
	GLuint fileVersion, type, numVertices, numIndices;
	GLint colourVal;

	GLuint loc = 0;

	// build mesh from vector
	fileVersion = (GLuint)buffer->at(++loc);
	type = (GLuint)buffer->at(loc);
	numVertices = (GLuint)buffer->at(++loc);
	numIndices = (GLuint)buffer->at(++loc);
	colourVal = (GLint)buffer->at(++loc);

	MGLvecv3* vertices = new MGLvecv3(numVertices);
	MGLvecv2* texCoords = new MGLvecv2(numVertices);
	MGLvecv3* normals = new MGLvecv3(numVertices);
	MGLvecv4* colours = new MGLvecv4(numVertices);
	MGLvecu* indices = new MGLvecu(numIndices);

	MGLMesh* mesh = new MGLMesh();

	mesh->SetNumIndices(numIndices);
	mesh->SetNumVertices(numVertices);
	mesh->SetType(type);

	// vertices
	for (GLuint i = 0; i < numVertices; ++i) {
		glm::vec3 vec;
		vec.x = buffer->at(++loc);
		vec.y = buffer->at(++loc);
		vec.z = buffer->at(++loc);
		vertices->at(i) = vec;
	}
	mesh->SetVertices(vertices);

	// TexCoords
	for (GLuint i = 0; i < numVertices; ++i) {
		glm::vec2 vec;
		vec.x = buffer->at(++loc);
		vec.y = buffer->at(++loc);
		texCoords->at(i) = vec;
	}
	mesh->SetTexCoords(texCoords);

	// Normals
	for (GLuint i = 0; i < numVertices; ++i) {
		glm::vec3 vec;
		vec.x = buffer->at(++loc);
		vec.y = buffer->at(++loc);
		vec.z = buffer->at(++loc);
		normals->at(i) = vec;
	}
	mesh->SetNormals(normals);

	// Colours 
	if (colourVal < 1)
	{
		glm::vec4 vec;
		if (colourVal == -1) {
			vec.x = buffer->at(++loc);
			vec.y = buffer->at(++loc);
			vec.z = buffer->at(++loc);
			vec.w = buffer->at(++loc);
		}
		else if (colourVal == 0) {
			vec = MGL::WHITE;
		}

		mesh->SetColours(colours);
		mesh->SetNewColours(vec, GL_FALSE);

	}
	else {
		for (GLuint i = 0; i < numVertices; ++i) {
			glm::vec4 vec;
			vec.x = buffer->at(++loc);
			vec.y = buffer->at(++loc);
			vec.z = buffer->at(++loc);
			vec.w = buffer->at(++loc);
			colours->at(i) = vec;
		}
		mesh->SetColours(colours);
	}

	// Indices
	for (GLuint i = 0; i < numIndices; ++i) {
		GLuint temp;
		temp = (GLuint)buffer->at(++loc);
		indices->at(i) = temp;
	}
	mesh->SetIndices(indices);

	if (bufferData)
		mesh->BufferAllData();

	vertices = nullptr;
	texCoords = nullptr;
	normals = nullptr;
	colours = nullptr;
	indices = nullptr;

	return mesh;
}

void MGLFile::SaveMeshToMGL(MGLMesh* mesh, std::string fileName, GLboolean saveColours) {
	// open file
	std::ofstream out; 

	try {
		out.open(fileName + ".mgl", std::ios::binary);
		MGLException_FileError::IsSuccessful(out.is_open(), fileName);
		MGLException_Null::IsSuccessful(mesh);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

	GLfloat val = 0;
	// file version
	val = (GLfloat)MGL_FILE_CURRENTVERSION;
	out.write((GLchar*)&val, sizeof(GLfloat));
	// type
	val = (GLfloat)mesh->GetType();
	out.write((GLchar*)&val, sizeof(GLfloat));
	// numVertices
	val = (GLfloat)mesh->GetNumVertices();
	out.write((GLchar*)&val, sizeof(GLfloat));
	// numIndices
	val = (GLfloat)mesh->GetNumIndices();
	out.write((GLchar*)&val, sizeof(GLfloat));
	// determine colour val
	GLfloat colourVal = (GLfloat)saveColours;
	if (colourVal) {
		// if all the colours are the same, set colourVal to -1
		if (std::adjacent_find(mesh->GetColours()->begin(), mesh->GetColours()->end(),
			std::not_equal_to<glm::vec4>()) == mesh->GetColours()->end()) {

			colourVal = -1;
		}
	}
	out.write((GLchar*)&colourVal, sizeof(GLfloat));

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

	if (colourVal > 1) {
		for (glm::vec4 const& vec : *mesh->GetColours()) {
			out.write((GLchar*)&vec.x, sizeof(GLfloat));
			out.write((GLchar*)&vec.y, sizeof(GLfloat));
			out.write((GLchar*)&vec.z, sizeof(GLfloat));
			out.write((GLchar*)&vec.w, sizeof(GLfloat));
		}
	}
	else if (colourVal == -1) {
		glm::vec4 vec = mesh->GetColours()->at(0);
		out.write((GLchar*)&vec.x, sizeof(GLfloat));
		out.write((GLchar*)&vec.y, sizeof(GLfloat));
		out.write((GLchar*)&vec.z, sizeof(GLfloat));
		out.write((GLchar*)&vec.w, sizeof(GLfloat));
	}

	for (GLuint const& ind : *mesh->GetIndices()) {
		GLfloat val = (GLfloat)ind;
		out.write((GLchar*)&val, sizeof(GLfloat));
	}
	
	out.close();
}

MGLMesh* MGLFile::LoadOBJ(std::string fileName, GLboolean bufferData) {
	// load file into stringstream
	stringstream* stream = LoadFileToSS(fileName);

	try {
		MGLException_Null::IsSuccessful(stream);
		MGLException_File_FileType::IsSuccessful(fileName, ".obj");
	}
	catch (MGLException_Null& e) {
		std::cerr << e.what() << ": STRINGSTREAM LoadOBJ " << std::endl;
		return new MGLMesh(MGL_MESH_TRIANGLE);
	}
	catch (MGLException_File_FileType& e) {
		std::cerr << e.what() << std::endl;
		return new MGLMesh(MGL_MESH_TRIANGLE);
	}

	MGLObjFileData* obj = new MGLObjFileData();
	// init data with *blanks* for defaults
	obj->inputVertices.push_back(glm::vec3(0, 0, 0));
	obj->inputNormals.push_back(glm::vec3(0, 0, 0));
	obj->inputTexCoords.push_back(glm::vec2(0, 0));

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
		MGLException_FileError::IsSuccessful(file.is_open(), fileName);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << std::endl;
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

std::size_t MGLFile::DetermineFileSize(const GLuint numVertices, const GLuint numIndices, const GLint colourVal) {
	GLuint vec4Size = 4;
	GLuint vec3Size = 3;
	GLuint vec2Size = 2;
	GLuint bytesInFloat = sizeof(GLfloat);
	GLuint bytesInGLuint = sizeof(GLuint);

	GLuint total = bytesInFloat * 5; // fileVersion, type, numVertices, numIndices, colourVal

	total += (numVertices * vec3Size * bytesInFloat); // vertices
	total += (numVertices * vec2Size * bytesInFloat); // texCoords
	total += (numVertices * vec3Size * bytesInFloat); // normals

	if (colourVal == -1) {
		total += (vec4Size * bytesInFloat); // colours
	}
	else if (colourVal > 0) {
		total += (colourVal * vec4Size * bytesInFloat); // colours
	}

	total += (numIndices * bytesInGLuint);

	return (std::size_t)total;
}