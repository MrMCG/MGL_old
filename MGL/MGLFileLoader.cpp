#include "stdafx.h"

#include "MGLFileLoader.h"
#include "MGLMesh.h"
#include "MGLExceptions.h"
#include "MGLLog.h"

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