#pragma once
#include <iostream>
#include <exception>
#include <sstream>
#include <stdexcept>
#include <string>


// MGLException_EXCEPTYPE

class MGLException : public std::runtime_error {
public:
	MGLException(const std::string& str)
		: std::runtime_error("\nMGLEXCEPTION ERROR - " + str){}

	virtual const char* what() const throw() {
		return std::runtime_error::what();
	}
};

/******	Common exceptions ******/

class MGLException_FileNotFound : public MGLException {
public:
	MGLException_FileNotFound(std::string file)
		: MGLException("FILE NOT FOUND : "+file) {}

	virtual const char* what() const throw() {
		return MGLException::what();
	}

	static void IsSuccessful(const int& success, std::string file) {
		if (success != 1) {
			throw MGLException_FileNotFound(file);
		}
	}
};

/******	Init exceptions ******/

class MGLException_Init_GLFW : public MGLException {
public:
	MGLException_Init_GLFW()
		: MGLException("INIT : GLFW"){}

	static void IsSuccessful(const int& success) {
		if (success != GL_TRUE) {
			throw MGLException_Init_GLFW();
		}
	}
};

class MGLException_Init_WINDOW : public MGLException {
public:
	MGLException_Init_WINDOW()
		: MGLException("INIT : WINDOW"){}

	static void IsSuccessful(const GLFWwindow* success) {
		if (success == nullptr) {
			throw MGLException_Init_WINDOW();
		}
	}
};

class MGLException_Init_GLEW : public MGLException {
public:
	MGLException_Init_GLEW()
		: MGLException("INIT : GLEW"){}

	static void IsSuccessful(const int& success) {
		if (success != GLEW_OK) {
			throw MGLException_Init_GLEW();
		}
	}
};

/****** Shader exceptions ******/

class MGLException_Shader_LINK : public MGLException {
public:
	MGLException_Shader_LINK()
		: MGLException("SHADER : LINK") {}

	virtual const char* what() const throw() {
		return MGLException::what();
	}

	static void IsSuccessful(const GLint& success) {
		if (success != GL_TRUE) {
			throw MGLException_Shader_LINK();
		}
	}
};

class MGLException_Shader_COMPILE : public MGLException {
public:
	MGLException_Shader_COMPILE()
		: MGLException("SHADER : COMPILE") {}

	virtual const char* what() const throw() {
		return MGLException::what();
	}

	static void IsSuccessful(const GLint& success) {
		if (success != GL_TRUE) {
			throw MGLException_Shader_COMPILE();
		}
	}
};

/******	Image exceptions ******/

class MGLException_STB : public MGLException {
public:
	MGLException_STB(std::string fN)
		: MGLException("STB : ERROR OPENING FILE - "+fN) {}

	virtual const char* what() const throw() {
		return MGLException::what();
	}

	static void IsSuccessful(const int& success, const std::string fileName) {
		if (success == 0) {
			throw MGLException_STB(fileName);
		}
	}


private:
	std::string str;
};

