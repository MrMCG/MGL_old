#pragma once
#include "stdafx.h"

class MGLMesh;
// Handles the loading of .obj and .mgl files
class MGLFile : public MGLSingleton<MGLFile> {
	friend class MGLSingleton < MGLFile >;
public:
	// Converts .obj to .mgl (better storage for MGLMesh) 
	// ISSUE WITH 4 VERTEX FACE! need to implement GL_TRIANGLE_FAN
	void ConvertOBJToMGL(std::string fileName, GLboolean saveColours);
	// Loads in a .obj file and returns a resultant MGLMesh 
	// (WORKS BEST WITH FACES WITH 3 VERTICES!)
	MGLMesh* LoadOBJ(std::string fileName, GLboolean bufferData = GL_TRUE);
	// Loads a .mgl file and returns a resultant MGLMesh
	MGLMesh* LoadMGL(std::string fileName, GLboolean bufferData = GL_TRUE);
	// Converts a MGLMesh into a given .mgl flie
	void SaveMeshToMGL(MGLMesh* mesh, std::string fileName, GLboolean saveColours = GL_TRUE);

protected:
	MGLFile(){}
	~MGLFile(){}
	
	// Gets a stringstream of a full file
	std::stringstream* LoadFileToSS(std::string fileName);
	// Created a MGLmesh based on info from .obj file
	MGLMesh* CreateMesh(MGLObjFileData* obj);
	// Handles indices loading of a .obj face
	void HandleOBJFace(std::string line, MGLObjFileData* obj);
	// Correctly adds vertex data to list
	GLuint AddOBJVertToList(MGLObjFileData* obj, MGLObjVertData& vert);
};

/* .mgl file structure */
/* Based on data from MGLMesh */
/* Saved in binary, size of GLfloat */
/*
	*FILE_START*
	type
	numVertices
	numIndices
	m_vertices.size
	m_texCoord.size
	m_normals.size
	m_colours.size
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
	*COLOURS* <-- OPTIONAL
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