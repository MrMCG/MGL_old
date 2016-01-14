#include "stdafx.h"
#include "MGLShader.h"

#include "MGLExceptions.h"
#include "MGLDebug.h"

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

	GLint shaderType = -1;

	// Assign compiled shader
	switch (type) {
	case GL_VERTEX_SHADER:
		shaderType = (GLint)MGL_SHADER_VERTEX; break;
	case GL_FRAGMENT_SHADER:
		shaderType = (GLint)MGL_SHADER_FRAGMENT; break;
	case GL_GEOMETRY_SHADER:
		shaderType = (GLint)MGL_SHADER_GEOMETRY; break;
	default: break;
	}

	try {
		MGLException_IsLessThan::Test(shaderType, (GLint)0); // test if shaderType is still -1
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << std::endl;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, "%s%s", e.what(), ": Shader Type Unknown");

		return;
	}

	std::string into;

	// Load shader from file
	{
		std::ifstream file;
		std::string temp;

		file.open(fileName.c_str());

		try {
			MGLException_FileError::Test(file.is_open(), fileName);
		}
		catch (MGLException& e) {
			//std::cerr << e.what() << std::endl;
			MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

			return;
		}

		while (!file.eof()){
			std::getline(file, temp);
			into += temp + "\n";
		}

	}

	m_shaders[shaderType] = Compile(into.c_str(), type);

	try {
		MGLException_IsZero::Test(m_shaders[shaderType]);
		message += " - SUCCESS"; // runs is MGLException is not thrown
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << std::endl;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());
		message += " - FAIL: TYPE ERROR";
	}

	MGLLogHandle->AddLog(MGL_LOG_MAIN, GL_TRUE, message.c_str());
}

GLuint MGLShader::Compile(const char* data, GLenum type) {
	// Generate and compile loaded shader
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &data, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	try {
		MGLException_Shader_COMPILE::Test(status);
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << std::endl;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

		GLchar error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		//std::cerr << error << std::endl;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, error);

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
	
	for (GLuint i = 0; i < MGL_SHADER_MAX; ++i) {
		if (m_shaders[i]) {
			glAttachShader(m_program, m_shaders[i]);
		}
	}

	glLinkProgram(m_program);

	try {
		GLint code = GL_FALSE;
		glGetProgramiv(m_program, GL_LINK_STATUS, &code);
		MGLException_Shader_LINK::Test(code);

		code = GL_FALSE;
		glValidateProgram(m_program);
		glGetProgramiv(m_program, GL_VALIDATE_STATUS, &code);
		MGLException_Shader_LINK::Test(code);

		SetDefaultAttributes();
	}
	catch (MGLException& e) {
		//std::cerr << e.what() << std::endl;
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, e.what());

		GLchar log[512];
		glGetProgramInfoLog(m_program, sizeof(log), NULL, log);
		MGLLogHandle->AddLog(MGL_LOG_ERROR, GL_TRUE, log);

		//std::cout << log << std::endl;
	}
}