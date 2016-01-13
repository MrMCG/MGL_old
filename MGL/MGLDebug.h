#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

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

