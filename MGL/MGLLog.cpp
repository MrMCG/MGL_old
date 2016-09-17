#include "stdafx.h"

#include "MGLLog.h"
#include "MGLExceptions.h"

const std::string MGLLog::LogDirectory = "_LOGS";
const std::string MGLLog::LogBasicFilename = "mglLog";
const std::string MGLLog::LogExtension = ".log";

MGLLog::MGLLog() {
	logsVector = new std::vector<MGLvecs*>(LogCount);
	enabledLogs = new MGLvecu(LogCount);

	for (GLuint i = 0; i < LogCount; ++i) {
		logsVector->at(i) = new MGLvecs;
		logsVector->at(i)->reserve(MaxLogSize);

		enabledLogs->at(i) = 1;
	}
}

MGLLog::~MGLLog() {
	for (auto* vec : *logsVector) {
		delete vec;
	}

	delete enabledLogs;
	delete logsVector;
}

void MGLLog::WriteToFile(const std::string fileName, const GLuint log, const GLboolean truncate) const {
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
	std::string intro = "******************************************\n";
	intro += "*************** New MGLLog ***************\n";
	intro += "******************************************\n";
	out.write((char *)&intro[0], intro.size());

	for (const std::string& str : *logsVector->at(log)) {
		out.write((char *)&str[0], str.size());
		out.write("\n", 1);
	}
}

void MGLLog::WriteLogsToFile(const GLboolean truncate) {
	std::string path = LogDirectory + LogBasicFilename;
	std::string filename = "";

	for (size_t i = 0; i < logsVector->size(); i++) {
		filename = path + std::to_string(i) + LogExtension;
		WriteToFile(filename, i, truncate);
	}
}

void MGLLog::AddLog(const GLuint log, const GLboolean timeStamp, const std::string line, ...) const {
	if (!enabledLogs->at(log))
		return;

	// get arguments
	va_list args;
	va_start(args, line);

	// create char buffer based on inputs
	GLchar buffer[MaxLineSize];
	GLint needed = vsnprintf_s(buffer, MaxLineSize -1, _TRUNCATE, line.c_str(), args);
	va_end(args);

	// determine buffer length and possibly truncate size
	GLint length = (needed < 0) ? MaxLineSize : needed;

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
		text += ":" + std::to_string(now.tm_sec);
		text += "] ";
	}	

	text += std::string(buffer, (size_t)length);

	logsVector->at(log)->push_back(text);
}

void MGLLog::Flush(const GLuint log) {
	logsVector->at(log)->clear();
}