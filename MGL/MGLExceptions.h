#pragma once
#include "stdafx.h"

// MGLException_EXCEPTYPE

/*
	*Most* MGLExceptions have a Test(PARAMS) function that
	will throw the error if those params are true to the error name.

	e.g.
	MGLException_IsEqual::Test(1,0) // WONT throw
	MGLException_IsEqual::Test(25,25) // throws
	MGLException_IsZero::Test(12) // WONT throw
	MGLException_IsZero::Test(0) // throws
*/

// Standard exception
class MGLException : public std::runtime_error {
public:
	MGLException(const std::string str)
		: std::runtime_error("MGLEXCEPTION ERROR - "+str+" "){}

	virtual const char* what() const throw();
};

/******	Common exceptions ******/

// Throws if file has error on open
class MGLException_FileError : public MGLException {
public:
	MGLException_FileError(const GLint fileReturnCode, std::string filename)
		: MGLException("FILE ERROR : "+ filename +" Returned code: " + std::to_string(fileReturnCode)) {}

	const char* what() const throw() override { return MGLException::what(); }

	static void Test(const GLint fileReturnCode, std::string filename);
};

// Throws if file size is smaller than expectedSize
class MGLException_FileTooSmall: public MGLException {
public:
	MGLException_FileTooSmall(const std::string file)
		: MGLException("FILE TOO SMALL : "+file+" ") {}

	const char* what() const throw() override {	return MGLException::what(); }

	static void Test(const std::string fileName, std::ifstream& file, const GLuint expectedSize);
};

// Throws if pointer given is nullptr
class MGLException_IsNullptr : public MGLException {
public:
	MGLException_IsNullptr()
		: MGLException("POINTER NULL : ") {}

	const char* what() const throw() override {	return MGLException::what(); }

	static void Test(const void* testPointer);
};

// Throws if given number is zero
class MGLException_IsZero : public MGLException {
public:
	MGLException_IsZero()
		: MGLException("IS ZERO : ") {}

	const char* what() const throw() override {	return MGLException::what(); }

	template<typename T>
	static void Test(const T number) {
		if (number == 0) {
			throw MGLException_IsZero();
		}
	}
};

// Throws if given number is NOT zero
class MGLException_IsNotZero : public MGLException {
public:
	MGLException_IsNotZero()
		: MGLException("NOT ZERO : ") {}

	const char* what() const throw() override { return MGLException::what(); }

	template<typename T>
	static void Test(const T number) {
		if (number != 0) {
			throw MGLException_IsNotZero();
		}
	}
};

// Throws if number is less than size
class MGLException_IsLessThan : public MGLException {
public:
	template<typename T, typename N>
	MGLException_IsLessThan(const T num, const N size)
		: MGLException("LESS THAN : "+std::to_string(num)+" < "+std::to_string(size)) {}

	const char* what() const throw() override {	return MGLException::what(); }

	template<typename T, typename N>
	static void Test(const T num, const N size) {
		if (num < size) {
			throw MGLException_IsLessThan(num, size);
		}
	}
};

// Throws if num1 != num2
class MGLException_IsNotEqual : public MGLException {
public:
	template<typename T, typename N>
	MGLException_IsNotEqual(const T num1, const N num2)
		: MGLException("NOT EQUAL : " + std::to_string(num1) + " " + std::to_string(num2)) {}

	const char* what() const throw() override {	return MGLException::what(); }

	template<typename T, typename N>
	static void Test(const T lhs, const N rhs) {
		if (lhs != rhs) {
			throw MGLException_IsNotEqual(lhs, rhs);
		}
	}
};

// Throws if num1 == num2
class MGLException_IsEqual : public MGLException {
public:
	template<typename T, typename N>
	MGLException_IsEqual(const T num1, const N num2)
		: MGLException("EQUAL : " + std::to_string(num1) + " " + std::to_string(num2)) {}

	const char* what() const throw() override {	return MGLException::what(); }

	template<typename T, typename N>
	static void Test(const T lfs, const N rhs) {
		if (lhs == rhs) {
			throw MGLException_IsEqual(lhs, rhs);
		}
	}
};

// Throws is the file given does not have the correct file extension
class MGLException_UnexpectedFileType : public MGLException {
public:
	MGLException_UnexpectedFileType(const std::string fileName, const std::string fileType)
		: MGLException("FILE TYPE : EXPECTING TYPE " + fileType + " GIVEN " + fileName + " ") {}

	const char* what() const throw() override {	return MGLException::what(); }

	static void Test(const std::string fileName, const std::string expectedExtension);
};

/******	Init exceptions ******/

// Throws if theres a problem with glfwInit() in MGLContext
class MGLException_Init_GLFW : public MGLException {
public:
	MGLException_Init_GLFW()
		: MGLException("INIT : GLFW : "){}

	static void Test(const GLint success) {
		if (success != GL_TRUE) {
			throw MGLException_Init_GLFW();
		}
	}
};

// Throws if theres a problem with glewInit from MGLContext::InitGL
class MGLException_Init_GLEW : public MGLException {
public:
	MGLException_Init_GLEW()
		: MGLException("INIT : GLEW "){}

	static void Test(const GLint success) {
		if (success != GLEW_OK) {
			throw MGLException_Init_GLEW();
		}
	}
};




