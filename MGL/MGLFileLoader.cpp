#include "stdafx.h"

#include "MGLFileLoader.h"
#include "MGLMesh.h"
#include "MGLExceptions.h"
#include "MGLLog.h"
#include "MGLTimer.h"

template<class T>
std::stringstream* MGLFileLoader<T>::LoadFileToSS(std::string fileName) {
	std::ifstream file(fileName, std::ios::in);

	if (!MGL::FileOpenedSuccessful(file, fileName))
		return nullptr;

	std::stringstream* stream = new std::stringstream();
	*stream << file.rdbuf();

	return stream;
}

template <class T>
template <class X>
std::vector<X>* MGLFileLoader<T>::LoadFileToVec(std::string fileName) {
	std::ifstream file(fileName, std::ios::in | std::ios::binary);

	if (!MGL::FileOpenedSuccessful(file, fileName))
		return nullptr;

	auto size = MGL::GetFilesize(file);

	// read file buffer into vector
	std::vector<X>* buffer = new std::vector < GLfloat >((size / sizeof(X)) + 1);
	file.read(reinterpret_cast<GLchar*>(&buffer->at(0)), size);

	return buffer;
}

template <class T>
void MGLFileLoader<T>::SaveMeshToMGL(MGLMesh* mesh, std::string fileName, GLboolean saveColours) {
	// open file
	std::ofstream out(fileName + ".mgl", std::ios::binary);

#ifdef MGL_USER_INCLUDE_FILETC
	try {
		MGLException_FileError::Test(out.is_open(), fileName);
		MGLException_IsNullptr::Test(mesh);
	}
	catch (MGLException_FileError& e) {
		//std::cerr << e.what() << std::endl;
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

		return;
	}
	catch (MGLException_IsNullptr& e) {
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%s", e.what(), "SMTM Mesh");

		return;
	}
#endif // MGL_USER_INCLUDE_FILETC

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

}








MGLMesh* MGLFileLoaderMGL::Load(std::string fileName) {
	MGLTimer timer = MGLTimer();
	timer.Start();

	std::vector<GLfloat>* buffer = LoadFileToVec<GLfloat>(fileName);

#ifdef MGL_USER_INCLUDE_FILETC
	try {
		// buffers not null
		MGLException_IsNullptr::Test(buffer);
		// buffer size is large enough (for DetermineFileSize())
		MGLException_IsLessThan::Test(buffer->size(), (std::size_t)MGL_FILELOADERMGL_BUFFERMINSIZE);
		// buffer size is what is expected
		MGLException_IsNotEqual::Test(DetermineFileSize(
			(GLuint)buffer->at(2), (GLuint)buffer->at(3), (GLint)buffer->at(4)) + 1, buffer->size());
	}
	catch (MGLException_IsNullptr& e) {
		//std::cerr << e.what() << ": FILE SIZE ERROR " << fileName << std::endl;
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%s", e.what(), ": Null Buffer");
		delete buffer;
		return new MGLMesh(MGL_MESH_TRIANGLE);
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << ": FILE SIZE ERROR " << fileName << std::endl;
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%s%s", e.what(), ": FILE SIZE ERROR ", fileName.c_str());
		delete buffer;
		return new MGLMesh(MGL_MESH_TRIANGLE);
	}
#endif // MGL_USER_INCLUDE_FILETC

	MGLMesh* mesh = LoadMesh(buffer);


	mesh->BufferAllData();

	delete buffer;

	timer.End();
	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "MGL Load: %s : %i vertices in %f", fileName.c_str(), mesh->GetNumVertices(), timer.GetTime());

	return mesh;
}

std::size_t MGLFileLoaderMGL::DetermineFileSize(const GLuint numVertices, const GLuint numIndices, const GLint colourVal) const {
	// number of floats in vectors
	GLuint vec4Size = 4;
	GLuint vec3Size = 3;
	GLuint vec2Size = 2;

	GLuint total = 5; // fileVersion, type, numVertices, numIndices, colourVal

	total += numVertices * (vec3Size * 2); // vertices, normals
	total += numVertices * vec2Size; // texCoords

	if (colourVal == -1) {
		total += vec4Size; // colours
	}
	else if (colourVal > 0) {
		total += colourVal * vec4Size; // colours
	}

	total += numIndices;

	return (std::size_t)total;
}

MGLMesh* MGLFileLoaderMGL::LoadMesh(const MGLvecf* buffer) {
	GLuint fileVersion, type, numVertices, numIndices;
	GLint colourVal;

	GLuint loc = 0;

	// build mesh from vector
	fileVersion = (GLuint)buffer->at(loc);
	type = (GLuint)buffer->at(++loc);
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

	vertices = nullptr;
	texCoords = nullptr;
	normals = nullptr;
	colours = nullptr;
	indices = nullptr;

	return mesh;
}