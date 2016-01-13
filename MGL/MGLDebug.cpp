#include "stdafx.h"
#include "MGLDebug.h"

#include "MGLExceptions.h"

/******************************/
/*********** MGLLog ***********/
/******************************/

MGLLog::MGLLog() {
	m_mainLog = new MGLvecs();
	m_errorLog = new MGLvecs();
}

MGLLog::~MGLLog() {
	delete m_mainLog;
	delete m_errorLog;
}

void MGLLog::WriteToFile(const std::string fileName, const GLboolean mainLog, const GLboolean truncate) {
	std::ofstream out(fileName, truncate ? std::ios::out | std::ios::trunc : std::ios::out | std::ios::app);

	// did it open successfully
	try {
		MGLException_FileError::Test(out.is_open(), fileName);
	}
	catch (MGLException& e) {
		std::cerr << e.what() << "MGLLOG WRTIE ERROR" << std::endl;
		return;
	}

	// write intro string
	std::string intro = "----- New MGLLog -----\n";
	out.write((char *)&intro[0], intro.size());

	// write each log line
	for (const std::string& str : mainLog ? *m_mainLog : *m_errorLog) {
		out.write((char *)&str[0], str.size());
		out.write("\n", 1);
	}

	out.close();
}

void MGLLog::AddLog(const GLboolean mainLog, const GLboolean timeStamp, const std::string line, ...) {
	// get arguments
	va_list args;
	va_start(args, line);

	// create char buffer based on inputs
	GLchar buffer[MGL_LOG_MAXLINESIZE];
	GLint needed = vsnprintf_s(buffer, MGL_LOG_MAXLINESIZE-1, _TRUNCATE, line.c_str(), args);
	va_end(args);

	// determine buffer length and possibly truncate size
	GLint length = (needed < 0) ? MGL_LOG_MAXLINESIZE : needed;

	// create timestamp
	std::string text = "";
	if (timeStamp) {
		std::time_t t = std::time(nullptr);
		struct tm now;
		localtime_s(&now, &t);
		text = "[" + std::to_string(now.tm_mday);
		text += "-" + std::to_string(now.tm_mon + 1);
		text += "-" + std::to_string(now.tm_year + 1900);
		text += " " + std::to_string(now.tm_hour);
		text += ":" + std::to_string(now.tm_min);
		text += "] ";
	}

	text += std::string(buffer, (size_t)length);

	// write to particular log
	if (mainLog)
		m_mainLog->push_back(text);
	else
		m_errorLog->push_back(text);
}

void MGLLog::Flush(const GLboolean mainLog) {
	if (mainLog)
		m_mainLog->clear();
	else
		m_errorLog->clear();
}