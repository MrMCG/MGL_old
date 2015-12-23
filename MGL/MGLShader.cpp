#include "stdafx.h"
#include "MGLShader.h"

#include <fstream>
#include <string>

MGLShader::MGLShader() {
	m_program = glCreateProgram();

	for (int i = 0; i < SHADER_MAX; ++i) {
		m_shaders[i] = 0;
	}
}

MGLShader::~MGLShader() {
	for (int i = 0; i < SHADER_MAX; ++i) {
		if (m_shaders[i]) {
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}
	}

	glDeleteProgram(m_program);
}

void MGLShader::LoadShader(std::string fileName, GLenum type) {
	std::cout << "Compiling Shader : " << fileName;

	std::string into;

	// Load shader from file
	{
		std::ifstream file;
		std::string temp;

		file.open(fileName.c_str());
		try {
			MGLException_FileNotFound::IsSuccessful(file.is_open(), fileName);
		}
		catch (MGLException& e) {
			std::cerr << std::endl << e.what() << std::endl;
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
		m_shaders[SHADER_VERTEX] = Compile(into.c_str(), type); break;
	case GL_FRAGMENT_SHADER:
		m_shaders[SHADER_FRAGMENT] = Compile(into.c_str(), type); break;
	case GL_GEOMETRY_SHADER:
		m_shaders[SHADER_GEOMETRY] = Compile(into.c_str(), type); break;
	default: std::cout << " - FAIL: TYPE ERROR" << std::endl; return;
	}

	std::cout << " - SUCCESS" << std::endl;
}

GLuint MGLShader::Compile(const char* data, GLenum type) {
	// Generate and compile loaded shader
	GLuint shader = glCreateShader(type);

	try {
		glShaderSource(shader, 1, &data, NULL);
		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		MGLException_Shader_COMPILE::IsSuccessful(status);
	}
	catch (MGLException& e) {
		std::cerr << std::endl << e.what() << std::endl;

		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		std::cerr << error << std::endl;

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void MGLShader::SetDefaultAttributes() {
	glBindAttribLocation(m_program, BUFFER_VERTEX, "position");
	glBindAttribLocation(m_program, BUFFER_COLOUR, "colour");
	glBindAttribLocation(m_program, BUFFER_TEXTURE, "texcoord");
}

void MGLShader::Link() {
	try {
		for (int i = 0; i < SHADER_MAX; ++i) {
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
		std::cerr << e.what() << std::endl;
		char log[512];
		glGetProgramInfoLog(m_program, sizeof(log), NULL, log);
		std::cout << log << std::endl;
	}
}