#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

class MGLMesh;

#define MGL_FILELOADER_DEFAULTFILEPATH "mglData"
#define MGL_FILELOADERMGL_BUFFERMINSIZE 5
#define MGL_FILELOADERMGL_CURRENTVERSION 1.0f

#define MGLI_FileLoaderMGL MGLFileLoaderMGL::Instance()

template <class T>
class MGLFileLoader {
public:

	virtual T* Load(std::string fileName) = 0;

	static void SaveMeshToMGL(MGLMesh* mesh, std::string fileName, GLboolean saveColours);

protected:

	std::stringstream* LoadFileToSS(std::string fileName);

	template < typename T >
	std::vector<T>* LoadFileToVec(std::string fileName);

};

class MGLFileLoaderMGL : public MGLFileLoader<MGLMesh>, public MGLSingleton<MGLFileLoaderMGL> {
	friend class MGLSingleton<MGLFileLoaderMGL>;

public:
	MGLMesh* Load(std::string fileName) override;

private:
	// Determines correct file size based on inputs
	std::size_t DetermineFileSize(const GLuint numVertices, const GLuint numIndices, const GLint colourVal) const;
	// Creates a MGLMesh from a loaded buffer
	static MGLMesh* LoadMesh(const MGLvecf* buffer);

	MGLFileLoaderMGL() {}
	MGLFileLoaderMGL(const MGLFileLoaderMGL&) = delete;
	MGLFileLoaderMGL(const MGLFileLoaderMGL&&) = delete;
	MGLFileLoaderMGL& operator=(const MGLFileLoaderMGL&) = delete;
	MGLFileLoaderMGL& operator=(const MGLFileLoaderMGL&&) = delete;
};

