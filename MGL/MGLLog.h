#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

#define MGL_LOG_MAXLINESIZE 512
#define MGL_LOG_MAXLOGSIZE 512
#define MGL_LOG_AMOUNT 2

#define MGL_LOG_DIRECTORY "_LOGS/"
#define MGL_LOG_FILENAME_MAIN MGL_LOG_DIRECTORY"mgl_main_log.txt"
#define MGL_LOG_FILENAME_ERROR MGL_LOG_DIRECTORY"mgl_error_log.txt"

#define MGL_LOG_ERROR 0
#define MGL_LOG_MAIN 1

#define MGLI_Log MGLLog::Instance()

class MGLLog : public MGLSingleton < MGLLog > {
	friend class MGLSingleton < MGLLog > ;
public:
	// Write current log to .txt
	void WriteToFile(const std::string fileName, const GLuint log,
		const GLboolean truncate = GL_FALSE) const;
	// Add string to log file, follows same rules as printf
	void AddLog(const GLuint log, const GLboolean timeStamp, const std::string line, ...) const;
	// Delete strings in current log (not the .txt file)
	void Flush(const GLuint log = 1);

	// Disables logging to particular log
	void Disable(const GLuint log) { enabledLogs->at(log) = 0; }
	// Enables logging to particular log
	void Enable(const GLuint log) { enabledLogs->at(log) = 1; }

	MGLvecs* GetLog(const GLuint index) const { return logsVector->at(index); }
protected:
	MGLLog();
	~MGLLog();

	/****** Data ******/

	std::vector<MGLvecs*>* logsVector;
	MGLvecu* enabledLogs;

	//MGLvecs* m_mainLog;
	//MGLvecs* m_errorLog;

	//MGLvecs* m_screenLog; TODO
};
