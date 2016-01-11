#pragma once
#include "stdafx.h"

class MGLLog : public MGLSingleton < MGLLog > {
	friend class MGLSingleton < MGLLog > ;
public:
	// Write current log to .txt
	void WriteToFile(const std::string fileName, GLboolean mainLog,
		const GLboolean truncate = GL_FALSE);
	// Add string to log file, follows same rules as printf
	void AddLog(const GLboolean mainLog, const GLboolean timeStamp, const std::string line, ...);
	// Delete strings in current log (not the .txt file)
	void Flush(const GLboolean mainLog = GL_TRUE);

	MGLvecs* GetMainLog() const { return m_mainLog; }
	MGLvecs* GetErrorLog() const { return m_errorLog; }
protected:
	MGLLog();
	~MGLLog();

	/****** Data ******/

	MGLvecs* m_mainLog;
	MGLvecs* m_errorLog;

	//MGLvecs* m_screenLog; TODO
};

// Test cases for debugging
#ifdef MGLDEBUG
namespace MGL_TESTS_ {
	// Test all funcitons and clean (delete) created files
	void MGL_TEST_ALL(const GLboolean cleanFiles = GL_TRUE);
	// Deletes files created by test cases
	void MGL_TEST_FileCleanup();

	GLuint MGL_TEST_MGLFILE();
	GLuint MGL_TEST_MGLFILE_COTM(); // ConvertOBJToMGL
	GLuint MGL_TEST_MGLFILE_LO(); // LadOBJ
	GLuint MGL_TEST_MGLFILE_LM(); // LadMGL
	GLuint MGL_TEST_MGLFILE_SMTM(); // SaveMeshToMGL

	const MGLvecs createdTestFiles = { // KEEP SIZE EVEN!
		MGL_TESTS_DIRECTORY"MGLFILE_LO_1", // 0
		".mgl",
		MGL_TESTS_DIRECTORY"MGLFILE_LO_2", // 2
		".mgl",
		MGL_TESTS_DIRECTORY"MGLFILE_SMTM_1", // 4
		".mgl",
		MGL_TESTS_DIRECTORY"MGLFILE_SMTM_2", // 6
		".mgl",
	};

	const enum Counters{ total, MGLFile, max };
	const enum FuncMax{
		MGLFileMax = 32
	};
}
#endif