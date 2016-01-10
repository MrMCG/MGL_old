#pragma once
#include "stdafx.h"

class MGLLog : public MGLSingleton < MGLLog > {
	friend class MGLSingleton < MGLLog > ;
public:
	void WriteToFile(const GLboolean mainLog = GL_TRUE,
		const GLboolean truncate = GL_FALSE, const GLboolean flushBuffer = GL_FALSE);

	void AddLog(const GLboolean mainLog, const GLboolean timeStamp, const std::string line, ...);

	void Flush(const GLboolean mainLog = GL_TRUE);

	MGLvecs* GetMainLog() const { return m_mainLog; }
	MGLvecs* GetErrorLog() const { return m_errorLog; }
protected:
	MGLLog();
	~MGLLog();

	MGLvecs* m_mainLog;
	MGLvecs* m_errorLog;
	//MGLvecs* m_screenLog; TODO
};