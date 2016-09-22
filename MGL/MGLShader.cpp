#include "stdafx.h"

#include "MGLShader.h"
#include "MGLExceptions.h"
#include "MGLLog.h"
#include "MGLMesh.h"

#include "MGLFileLoaderMGL.h"

MGLShader::MGLShader() {
	program = glCreateProgram();

	for (GLuint i = 0; i < MaxShaders; ++i) {
		shaders[i] = 0;
	}
}

MGLShader::~MGLShader() {
	for (GLuint i = 0; i < MaxShaders; ++i) {
		if (shaders[i]) {
			glDetachShader(program, shaders[i]);
			glDeleteShader(shaders[i]);
		}
	}
	glDeleteProgram(program);
}

void MGLShader::LoadShader(const std::string fileName, const GLenum glType) {
	std::string message = "Compiling Shader : "+fileName;

	GLint shaderType = AssignShaderType(glType);

	if (!IsValidShaderType(shaderType)) 
		return;

	std::stringstream* stream = LoadShaerFromFile(fileName);

	if (!CompileShader(stream, glType, shaderType)) {
		delete stream;
		return;
	}

	delete stream;
	message += " - SUCCESS";

	MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, message.c_str());
}

GLuint MGLShader::CompileGLShader(const char* data, const GLenum glType){

	GLuint shader = glCreateShader(glType);

	glShaderSource(shader, 1, &data, nullptr);
	glCompileShader(shader);

	GLint status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	try {
		MGLException_IsNotEqual::Test(status, GL_TRUE);
	}
	catch (MGLException& e) {
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());
		LogGLShaderError(shader);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void MGLShader::SetDefaultAttributes() {
	glBindAttribLocation(program, MGLMesh::Vertex, MGLMesh::VertexUniform);
	glBindAttribLocation(program, MGLMesh::Textures, MGLMesh::TextureUniform);
	glBindAttribLocation(program, MGLMesh::Normals, MGLMesh::NormalsUniform);
	glBindAttribLocation(program, MGLMesh::Colours, MGLMesh::ColourUniform);
}

GLboolean MGLShader::CompileShader(const std::stringstream* stream, const GLenum glType, const GLint mglType) {

	const std::string& fileString = stream->str();
	const char* cFileString = fileString.c_str();

	shaders[mglType] = CompileGLShader(cFileString, glType);
	
	try {
		MGLException_IsZero::Test(shaders[mglType]);
	}
	catch (MGLException& e) {
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());
		return GL_FALSE;
	}
	return GL_TRUE;
}

GLboolean MGLShader::IsValidShaderType(const GLint type) const {
	try {
		MGLException_IsEqual::Test(type, Invalid);
	}
	catch (MGLException& e) {
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%s", e.what(), ": Shader Type Invalid");
		return GL_FALSE;
	}
	return GL_TRUE;
}

GLint MGLShader::AssignShaderType(const GLenum glType) const
{
	switch (glType) {
	case GL_VERTEX_SHADER:
		return Vertex;
	case GL_FRAGMENT_SHADER:
		return Fragment;
	case GL_GEOMETRY_SHADER:
		return Geometry;
	default:
		return -1;
	}
}

std::stringstream* MGLShader::LoadShaerFromFile(const std::string filename) const {
	
	std::stringstream* stream = MGLI_FileLoaderMGL->LoadFileToSS(filename);
	return stream;
}

void MGLShader::LogGLShaderError(const GLuint shader) {
	GLchar error[512];
	glGetInfoLogARB(shader, sizeof(error), nullptr, error);
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, error);
}

void MGLShader::LogGLProgramError(const GLuint prgrm) {
	GLchar error[512];
	glGetProgramInfoLog(prgrm, sizeof(error), nullptr, error);
	MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, error);
}

void MGLShader::Link() {
	
	for (GLuint i = 0; i < MaxShaders; ++i) {
		if (shaders[i]) {
			glAttachShader(program, shaders[i]);
		}
	}

	glLinkProgram(program);

	try {
		GLint code = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &code);
		MGLException_IsNotEqual::Test(code, GL_TRUE);

		code = GL_FALSE;
		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &code);
		MGLException_IsNotEqual::Test(code, GL_TRUE);

		SetDefaultAttributes();
	}
	catch (MGLException& e) {
		MGLI_Log->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

		LogGLProgramError(program);
	}
}
