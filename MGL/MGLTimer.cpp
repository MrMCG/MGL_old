#include "stdafx.h"

#include "MGLTimer.h"

MGLTimer::MGLTimer() {
	Start();
	End();
}

void MGLTimer::Start() {
	start = static_cast<GLdouble>(glfwGetTime());
}

void MGLTimer::End() {
	end = static_cast<GLdouble>(glfwGetTime());
	CalcTime();
}

GLdouble MGLTimer::GetTime() const {
	return time;
}

GLfloat MGLTimer::GetTimef() const {
	return static_cast<GLfloat>(GetTime());
}

void MGLTimer::CalcTime() {
	time = end - start;
}
