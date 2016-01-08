#pragma once
#include "stdafx.h"

class MGLMesh;
class MGLFile : public MGLSingleton<MGLFile> {
	friend class MGLSingleton < MGLFile >;
public:
	void ConvertOBJToMGL(std::string fileName);
	MGLMesh* LoadOBJ(std::string fileName, GLboolean buffer = GL_TRUE);
	MGLMesh* LoadMGL(std::string fileName, GLboolean buffer = GL_TRUE);
protected:
	MGLFile(){}
	~MGLFile(){}

	std::stringstream* LoadFileToSS(std::string fileName);
	MGLMesh* CreateMesh(MGLObjFileData* obj);
	void HandleOBJFace(std::string line, MGLObjFileData* obj);
	GLuint AddOBJVertToList(MGLObjFileData* obj, MGLObjVertData& vert);
};

#define MGLFileHandle MGLFile::Instance()