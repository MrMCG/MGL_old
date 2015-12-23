#include "stdafx.h"
#pragma once

class MGLShader {
public:
	MGLShader();
	virtual ~MGLShader();

	// Loads and compiles shader, given file and shader type
	void LoadShader(std::string fileName, GLenum type);
	// Links all viable shaders
	void Link();

	// Uses shaders program
	void Use() const { glUseProgram(m_program); }

protected:
	// Returns 0 if error, otherwise returns shader
	virtual GLuint Compile(const char* data, GLenum type);
	// Sets default uniforms
	virtual void SetDefaultAttributes();

	GLuint m_program;
	GLuint m_shaders[SHADER_MAX];
};