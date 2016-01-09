#pragma once

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

	static void IsSuccessful(const GLint& success, std::string file) {
		if (success != 1) {
			throw MGLException_FileNotFound(file);
		}
	}
};

class MGLException_SizeTooSmall: public MGLException {
public:
	MGLException_SizeTooSmall(std::string file)
		: MGLException("FILE TOO SMALL : " + file) {}

	virtual const char* what() const throw() {
		return MGLException::what();
	}

	static void IsSuccessful(const GLuint& size, GLuint minSize, std::string file) {
		if (size < minSize) {
			throw MGLException_SizeTooSmall(file);
		}
	}
};

class MGLException_NullError : public MGLException {
public:
	MGLException_NullError()
		: MGLException("ITEM NULL") {}

	virtual const char* what() const throw() {
		return MGLException::what();
	}

	static void IsSuccessful(const void* success) {
		if (success == nullptr) {
			throw MGLException_NullError();
		}
	}
};

/******	Init exceptions ******/

class MGLException_Init_GLFW : public MGLException {
public:
	MGLException_Init_GLFW()
		: MGLException("INIT : GLFW"){}

	static void IsSuccessful(const GLint& success) {
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

	static void IsSuccessful(const GLint& success) {
		if (success != GLEW_OK) {
			throw MGLException_Init_GLEW();
		}
	}
};

/****** MGLShader exceptions ******/

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

	static void IsSuccessful(const GLint& success, const std::string fileName) {
		if (success == 0) {
			throw MGLException_STB(fileName);
		}
	}
};

/******	MGLFile exceptions ******/

class MGLException_File_LoadOBJ : public MGLException {
public:
	MGLException_File_LoadOBJ()
		: MGLException("MGLFILE : LoadOBJ") {}

	virtual const char* what() const throw() {
		return MGLException::what();
	}
};

class MGLException_File_FileType : public MGLException {
public:
	MGLException_File_FileType(const std::string& fileName, const std::string& fileType)
		: MGLException("MGLFILE : EXPECTING TYPE "+fileType+" GIVEN "+fileName) {}

	virtual const char* what() const throw() {
		return MGLException::what();
	}

	static void IsSuccessful(const std::string& fileName, const std::string& fileType) {
		std::string ext = fileName.substr(fileName.length() - fileType.length(), fileName.length());
		if (ext != fileType) {
			throw MGLException_File_FileType(fileName, fileType);
		}
	}
};