#include "stdafx.h"
#include "MGLTexture.h"

MGLTexture::MGLTexture() {
	m_textures = new std::vector<GLuint>;
	m_names = new std::vector<std::string>;
	m_types = new std::vector<MGLenum>;

	LoadTexture(MGL_DEFAULT_TEXTURE1, "DEFAULT", MGL_TEXTURE_DIFFUSE);
}

MGLTexture::~MGLTexture() {
	for (GLuint tex : *m_textures) {
		glDeleteTextures(1, &tex);
	}
	delete m_textures;

	m_names->clear();
	delete m_names;

	m_types->clear();
	delete m_types;
}

void MGLTexture::AddTexture(GLuint tex, std::string name, MGLenum type) {
	if (FindTexture(tex))
		return;

	m_textures->push_back(tex);
	m_names->push_back(name);
}

void MGLTexture::LoadTexture(std::string fileName, std::string name, 
		MGLenum type, GLboolean repeat, GLboolean linear) {
	GLuint tex = MGL::LoadTextureFromFile(fileName);
	MGL::SetTextureParameters(tex, repeat, linear);

	AddTexture(tex, name, type);
}

GLuint MGLTexture::GetTexture(std::string name, GLboolean returnIndex) {
	for (GLuint i = 0; i < m_names->size(); ++i) {
		if (m_names->at(i) == name) {
			if (returnIndex)
				return i;
			else
				return m_textures->at(i);
		}
	}

	return 0;
}

GLuint MGLTexture::GetTextureType(std::string name) {
	return m_textures->at(GetTexture(name, GL_TRUE));
}

GLuint MGLTexture::GetTexture(GLuint index) {
	return m_textures->at(index);
}

GLuint MGLTexture::GetTextureType(GLuint index) {
	return m_types->at(index);
}

GLuint MGLTexture::FindTexture(GLuint tex) {
	for (GLuint i = 0; i < m_textures->size(); ++i) {
		if (m_textures->at(i) == tex)
			return i;
	}

	return 0;
}


