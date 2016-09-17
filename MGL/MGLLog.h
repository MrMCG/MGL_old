#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

#define MGL_LOG_DIRECTORY "_LOGS/"
#define MGL_LOG_FILENAME_MAIN MGL_LOG_DIRECTORY"mgl_main_log.txt"
#define MGL_LOG_FILENAME_ERROR MGL_LOG_DIRECTORY"mgl_error_log.txt"

#define MGL_LOG_ERROR 0
#define MGL_LOG_MAIN 1

#define MGLI_Log MGLLog::Instance()

class MGLLog : public MGLSingleton < MGLLog > {
	friend class MGLSingleton < MGLLog > ;
public:

	void WriteToFile(const std::string fileName, const GLuint log,
		const GLboolean truncate = GL_FALSE) const;
	void WriteLogsToFile(const GLboolean truncate = GL_FALSE);

	void AddLog(const GLuint log, const GLboolean timeStamp, const std::string line, ...) const;

	void Flush(const GLuint log = 1);


	void Disable(const GLuint log) { enabledLogs->at(log) = 0; }

	void Enable(const GLuint log) { enabledLogs->at(log) = 1; }

	MGLvecs* GetLog(const GLuint index) const { return logsVector->at(index); }

	void WriteMGLLogSettings() const;
	void WriteMGLLogDataStats() const;
protected:
	MGLLog();
	~MGLLog();

	std::vector<MGLvecs*>* logsVector;
	MGLvecu* enabledLogs;


private:

	static const std::string LogDirectory;
	static const std::string LogBasicFilename;
	static const std::string LogExtension;
	enum LogInfo {MaxLineSize = 512, MaxLogSize = 512, LogCount = 2, };
};

