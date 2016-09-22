#pragma once
#include "MGLUtil.h"

#include "MGLFileLoader.h"

/*
 * TODO: Finish this class
 */

class MGLFileLoaderString : public MGLFileLoader<std::string>, public MGLSingleton<MGLFileLoaderString> {
	friend class MGLSingleton<MGLFileLoaderString>;
public:

	std::string* Load(std::string fileName) override; // TODO

private:

	MGLFileLoaderString() {}
	MGLFileLoaderString(const MGLFileLoaderString&) = delete;
	MGLFileLoaderString(const MGLFileLoaderString&&) = delete;
	MGLFileLoaderString& operator=(const MGLFileLoaderString&) = delete;
	MGLFileLoaderString& operator=(const MGLFileLoaderString&&) = delete;
};