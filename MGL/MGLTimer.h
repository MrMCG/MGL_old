#pragma once
#include "stdafx.h"

// Timer in milliseconds
class MGLTimer {
public:
	MGLTimer();
	~MGLTimer() {};

	void Start();
	void End();

	GLdouble GetTime() const;
	GLfloat GetTimef() const;

private:

	void CalcTime();

	GLdouble start = 0;
	GLdouble end = 0;
	GLdouble time = 0;
};