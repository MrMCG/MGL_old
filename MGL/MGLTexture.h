#pragma once
#include "stdafx.h"

#include <vector>

class MGLTexture : public MGLSingleton<MGLTexture> {
	friend class MGLSingleton < MGLTexture > ;
public:
	void AddTexture(GLuint tex, std::string name, MGLenum type);

	void LoadTexture(std::string fileName, std::string name, MGLenum type,
		GLboolean repeat = GL_TRUE, GLboolean linear = GL_TRUE);

	GLuint GetTexture(std::string name, GLboolean returnIndex = GL_FALSE);
	GLuint GetTextureType(std::string name);

	GLuint GetTexture(GLuint index);
	GLuint GetTextureType(GLuint index);

protected:
	GLuint FindTexture(GLuint tex);

	MGLTexture();
	~MGLTexture();

	std::vector<GLuint>* m_textures;
	std::vector<std::string>* m_names;
	std::vector<MGLenum>* m_types;
};

#define MGLTexHandle MGLTexture::Instance()