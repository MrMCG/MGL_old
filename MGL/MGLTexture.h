#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

// TODO: make adding/loading textures handle 
// loading existing textures safer

// TODO: make adding/loading textures handle 
// loading existing textures safer

class MGLTexture : public MGLSingleton<MGLTexture> {
	friend class MGLSingleton < MGLTexture > ;
public:
	// Add a texture
	void AddTexture(GLuint tex, std::string name, MGLenum type);
	// Load a texture from file and apply attributes
	void LoadTexture(std::string fileName, std::string name, MGLenum type,
		GLboolean alpha = GL_FALSE, GLboolean repeat = GL_TRUE, GLboolean linear = GL_TRUE);
	// Get texture by name (return tex refrence or index in vector)
	GLuint GetTexture(std::string name, GLboolean returnIndex = GL_FALSE);
	GLuint GetTexture(GLuint index);
	// Get texture type
	GLuint GetTextureType(std::string name);
	GLuint GetTextureType(GLuint index);

protected:

	/****** Methods ******/

	GLuint FindTexture(GLuint tex);

	MGLTexture();
	~MGLTexture();

	/****** Data ******/

	MGLvecu* m_textures; // Texture refrences
	MGLvecs* m_names; // User given string id
	MGLvecm* m_types; // Type of texture
};

