#include "stdafx.h"
#include "MGLDebug.h"

#include <ctime>

MGLLog::MGLLog() {
	m_mainLog = new MGLvecs();
	m_errorLog = new MGLvecs();
}

MGLLog::~MGLLog() {
	WriteToFile(GL_FALSE);
	delete m_mainLog;
	delete m_errorLog;
}

void MGLLog::WriteToFile(const GLboolean mainLog, const GLboolean truncate, const GLboolean flushBuffer) {
	std::string fileName = "";

	if (mainLog)
		fileName = MGL_LOG_MAIN;
	else
		fileName = MGL_LOG_ERROR;
	
	std::ofstream out;

	try {
		out.open(fileName, truncate ? std::ios::out | std::ios::trunc : std::ios::out | std::ios::app);
		MGLException_FileError::IsSuccessful(out.is_open(), fileName);

	}
	catch (MGLException& e) {
		std::cerr << e.what() << "MGLLOG WRTIE ERROR" << std::endl;
		return;
	}

	std::string intro = "----- New MGLLog -----\n";

	out.write((char *)&intro[0], intro.size());

	for (const std::string& str : mainLog ? *m_mainLog : *m_errorLog) {
		out.write((char *)&str[0], str.size());
		out.write("\n", 2);
	}

	out.close();

	if (flushBuffer && mainLog)
		Flush();
	else if (flushBuffer)
		Flush(GL_FALSE);
		
}

void MGLLog::AddLog(const GLboolean mainLog, const GLboolean timeStamp, const std::string line, ...) {
	va_list args;
	va_start(args, line);

	GLchar buffer[MGL_LOG_MAXLINESIZE];
	GLint needed = vsnprintf_s(buffer, MGL_LOG_MAXLINESIZE-1, _TRUNCATE, line.c_str(), args);
	va_end(args);

	GLint length = (needed < 0) ? MGL_LOG_MAXLINESIZE : needed;

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