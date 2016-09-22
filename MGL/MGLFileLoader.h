#pragma once
#include "stdafx.h"

#define MGL_FILELOADER_DEFAULTFILEPATH "mglData"

class MGLMesh;

/*
 * TODO: Fix LoadFilesTo* for more classes
 */

template <class T>
class MGLFileLoader {
public:
	virtual ~MGLFileLoader() {}

	virtual T* Load(std::string fileName) = 0;

	static void SaveMeshToMGL(MGLMesh* mesh, std::string fileName, GLboolean saveColours);

	std::stringstream* LoadFileToSS(std::string fileName);

	template < class X >
	std::vector<X>* LoadFileToVec(std::string fileName);

};

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
