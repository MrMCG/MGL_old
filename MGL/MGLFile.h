#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

#define MGL_FILE_BUFFERMINSIZE 5
#define MGL_FILE_CURRENTVERSION 1.0f

#define MGLI_FileOBJ MGLFileOBJ::Instance()

/*
	TODO: Add multithreaded obj loader
	TODO: Add grouping/multiple mesh loading to obj loader
*/

class MGLMesh;
struct MGLObjVertData;
struct MGLObjFileData;

class MGLFileTEMP {
public:
	// Loads file and returns correct MGLMesh, buffered or unbuffered
	virtual MGLMesh* Load(std::string fileName, GLboolean bufferData = GL_TRUE) = 0;
	// Saves a given mesh to .mgl format
	void SaveMeshToMGL(MGLMesh* mesh, std::string fileName, GLboolean saveColours);

protected:
	MGLFileTEMP(std::string ext) : m_fileEXT(ext) {}
	virtual ~MGLFileTEMP(){}

	/****** Methods ******/

	// Gets a istringstream of a full file
	std::stringstream* LoadFileToSS(std::string fileName);
	// Loads binary file and returns a vector of required type
	template < typename T > 
	std::vector<T>* LoadFileToVec(std::string fileName);
	
	/****** Data ******/

	std::string m_fileEXT;
};

/* OBJ */

class MGLFileOBJ : public MGLFileTEMP, public MGLSingleton<MGLFileOBJ> {
	friend class MGLSingleton < MGLFileOBJ >;
public:
	virtual MGLMesh* Load(std::string fileName, GLboolean bufferData = GL_TRUE);

protected:
	MGLFileOBJ() : MGLFileTEMP(".obj") {}
	virtual ~MGLFileOBJ(){}

	GLboolean HandleOBJFace(std::string line, MGLObjFileData* obj);
	GLuint AddOBJVertToList(MGLObjFileData* obj, MGLObjVertData& vert);
	MGLMesh* CreateMesh(MGLObjFileData* obj);

	// Handle OBJ face with no normals
	void AddFaceMissing(MGLObjFileData* obj, MGLvecu& indices, const GLboolean isPolygon, const GLboolean skipTex);
	// Handle OBJ face
	void AddFace(MGLObjFileData* obj, MGLvecu& indices, const GLboolean isPolygon);
};

/* MGL */

class MGLFileMGL : public MGLFileTEMP, public MGLSingleton<MGLFileMGL> {
	friend class MGLSingleton < MGLFileMGL >;
public:
	virtual MGLMesh* Load(std::string fileName, GLboolean bufferData = GL_TRUE);

protected:
	MGLFileMGL() : MGLFileTEMP(".mgl") {}
	virtual ~MGLFileMGL(){}

	/****** Methods ******/

	// Determines correct file size based on inputs
	std::size_t DetermineFileSize(const GLuint numVertices, const GLuint numIndices, const GLint colourVal);
	// Creates a MGLMesh from a loaded buffer
	MGLMesh* LoadMesh(const MGLvecf* buffer);
};























/* .mgl file structure */
/* Based on data from MGLMesh */
/* Saved in binary, size of GLfloat */
/*
	*FILE_START*
	file verison
	type
	numVertices
	numIndices
	*COLOUR USE*
		-1 = one colour for all vertices
		0 = no colours
		>0 = number of colours
	*VERTICES*
		vec1.x
		vec1.y
		vec1.z
		vec2.x
		vec2.y
		...
	*TEXCOORDS*
		vec1.x
		vec1.y
		vec2.x
		vec2.y
		vec3.x
		...
	*NORMALS*
		vec1.x
		vec1.y
		vec1.z
		vec2.x
		vec2.y
		...
	*COLOURS* <-- optional, based on *COLOUR USE*
		vec1.x
		vec1.y
		vec1.z
		vec1.w
		vec2.x
		...
	*INDICES*
		ind1
		in2
		ind3
		ind4
		...
	*FILE_END*
*/