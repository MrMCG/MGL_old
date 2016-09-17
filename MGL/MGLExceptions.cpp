#include "stdafx.h"

#include "MGLExceptions.h"
#include "MGLUtil.h"

const char* MGLException::what() const throw() {
	return std::runtime_error::what();
}

void MGLException_FileError::Test(const GLint fileReturnCode, std::string filename) {
	if (fileReturnCode != 1) {
		throw MGLException_FileError(fileReturnCode, filename);
	}
}

void MGLException_FileTooSmall::Test(const std::string fileName, std::ifstream& file, const GLuint expectedSize) {
	auto fileSize = MGL::GetFilesize(file);

	if (fileSize < expectedSize) {
		throw MGLException_FileTooSmall(fileName);
	}
}

void MGLException_IsNullptr::Test(const void* testPointer) {
	if (testPointer == nullptr) {
		throw MGLException_IsNullptr();
	}
}

void MGLException_UnexpectedFileType::Test(const std::string fileName, const std::string expectedExtension) {

	auto fnLength = fileName.length();
	auto eeLength = expectedExtension.length();

	if (fnLength < eeLength) {
		throw MGLException_UnexpectedFileType(fileName, expectedExtension);
	}
	auto ext = fileName.substr(fnLength - eeLength, fileName.length());
	if (ext != expectedExtension) {
		throw MGLException_UnexpectedFileType(fileName, expectedExtension);
	}
}