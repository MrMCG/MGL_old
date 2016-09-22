#pragma once
#include "stdafx.h"

// TODO: Add capability to hold multiple shader programs

class MGLShader {
public:
	MGLShader();
	~MGLShader();

	void LoadShader(const std::string fileName, const GLenum type);

	void Link();
	void Use() const { glUseProgram(program); }

	GLuint Program() const { return program; }

	static GLuint CompileGLShader(const char* data, const GLenum glType);

private:

	enum ShaderType { Invalid = -1, Vertex, Fragment, Geometry, MaxShaders };

	void SetDefaultAttributes();

	GLboolean CompileShader(const std::stringstream* stream, const GLenum glType, const GLint mglType);
	GLboolean IsValidShaderType(GLint mglType) const;
	GLint AssignShaderType(const GLenum glType) const;
	std::stringstream* LoadShaerFromFile(const std::string filename) const;

	static void LogGLShaderError(GLuint shader);
	static void LogGLProgramError(GLuint program);

	GLuint program = 0;
	GLuint shaders[MaxShaders];

};