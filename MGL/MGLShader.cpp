#include "stdafx.h"
#include "MGLShader.h"

#include <fstream>
#include <string>

MGLShader::MGLShader() {
	m_program = glCreateProgram();

	for (GLuint i = 0; i < MGL_SHADER_MAX; ++i) {
		m_shaders[i] = 0;
	}
}

MGLShader::~MGLShader() {
	for (GLuint i = 0; i < MGL_SHADER_MAX; ++i) {
		if (m_shaders[i]) {
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}
	}
	glDeleteProgram(m_program);
}

void MGLShader::LoadShader(std::string fileName, GLenum type) {
	std::string message = "Compiling Shader : "+fileName;

	std::string into;

	// Load shader from file
	{
		std::ifstream file;
		std::string temp;

		file.open(fileName.c_str());

		try {
			MGLException_FileError::IsSuccessful(file.is_open(), fileName);
		}
		catch (MGLException& e) {
			//std::cerr << e.what() << std::endl;
			MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

			return;
		}

		while (!file.eof()){
			std::getline(file, temp);
			into += temp + "\n";
		}

		file.close();
	}

	// Assign compiled shader
	switch (type) {
	case GL_VERTEX_SHADER:
		m_shaders[MGL_SHADER_VERTEX] = Compile(into.c_str(), type); break;
	case GL_FRAGMENT_SHADER:
		m_shaders[MGL_SHADER_FRAGMENT] = Compile(into.c_str(), type); break;
	case GL_GEOMETRY_SHADER:
		m_shaders[MGL_SHADER_GEOMETRY] = Compile(into.c_str(), type); break;
	default: message += " - FAIL: TYPE ERROR"; return;
	}

	message += " - SUCCESS";
	MGLLodHandle->AddLog(MGL_LOG_MAIN, GL_TRUE, message.c_str());

}

GLuint MGLShader::Compile(const char* data, GLenum type) {
	// Generate and compile loaded shader
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &data, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	try {
		MGLException_Shader_COMPILE::IsSuccessful(status);
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << std::endl;
		MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

		GLchar error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		//std::cerr << error << std::endl;
		MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, error);

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void MGLShader::SetDefaultAttributes() {
	glBindAttribLocation(m_program, MGL_BUFFER_VERTEX, "position");
	glBindAttribLocation(m_program, MGL_BUFFER_TEXTURES, "texCoord");
	glBindAttribLocation(m_program, MGL_BUFFER_NORMALS, "normals");
	glBindAttribLocation(m_program, MGL_BUFFER_COLOURS, "colour");
}

void MGLShader::Link() {
	try {
		for (GLuint i = 0; i < MGL_SHADER_MAX; ++i) {
			if (m_shaders[i]) {
				glAttachShader(m_program, m_shaders[i]);
			}
		}
	
		glLinkProgram(m_program);

		GLint code = GL_FALSE;
		glGetProgramiv(m_program, GL_LINK_STATUS, &code);
		MGLException_Shader_LINK::IsSuccessful(code);

		code = GL_FALSE;
		glValidateProgram(m_program);
		glGetProgramiv(m_program, GL_VALIDATE_STATUS, &code);
		MGLException_Shader_LINK::IsSuccessful(code);

		SetDefaultAttributes();
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << std::endl;
		MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

		GLchar log[512];
		glGetProgramInfoLog(m_program, sizeof(log), NULL, log);
		MGLLodHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, log);

		//std::cout << log << std::endl;
	}
}