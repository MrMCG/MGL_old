#pragma once
#include "stdafx.h"

#include "MGLUtil.h"
#include "MGLFileLoader.h"

#define MGL_FILELOADERMGL_BUFFERMINSIZE 5
#define MGL_FILELOADERMGL_CURRENTVERSION 1.0f

#define MGLI_FileLoaderMGL MGLFileLoaderMGL::Instance()

class MGLMesh;

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

