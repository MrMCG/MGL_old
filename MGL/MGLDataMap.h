#pragma once
#include "stdafx.h"

template <typename T>
class MGLDataMap {
public:

	MGLDataMap(T* defaultMember);
	~MGLDataMap();

	void Register(std::string key, T* member);
	T* Get(std::string key) const;
	GLboolean Exists(std::string key) const;

	T* DefaultMember() const;
	void DeleteAll();

	T* operator[](const std::string& key);
	const T* operator[](const std::string& key) const;

private:

	std::unordered_map<std::string, T*>* dataMap = new std::unordered_map<std::string, T*>();
	static const std::string DefaultString;
};

/* Implementation */

template <class T>
const std::string MGLDataMap<T>::DefaultString = "DEFAULT";

template <class T>
MGLDataMap<T>::MGLDataMap(T* defaultMember) {
	Register(DefaultString, defaultMember);
}

template <class T>
MGLDataMap<T>::~MGLDataMap() {
	delete dataMap;
}

template <class T>
void MGLDataMap<T>::Register(std::string key, T* member) {
	dataMap->insert(std::make_pair(key, member));
}

template <class T>
T* MGLDataMap<T>::Get(std::string key) const {
	if (Exists(key))
		return dataMap->at(key);
	else
		return DefaultMember();
}

template <class T>
T* MGLDataMap<T>::DefaultMember() const {
	return dataMap->at(DefaultString);
}

template <class T>
GLboolean MGLDataMap<T>::Exists(std::string key) const {
	try {
		MGLException_IsNotZero::Test(dataMap->count(key));
	}
	catch (MGLException& e) {
		MGLI_Log->AddLog(MGL_LOG_MAIN, GL_TRUE, "MGLDataMap : Key already exists " + std::string(e.what()));
		return GL_TRUE;
	}
	return GL_FALSE;
}

template <class T>
void MGLDataMap<T>::DeleteAll() {
	for (auto& pair : *dataMap) {
		delete pair.second;
	}
}

template <typename T>
T* MGLDataMap<T>::operator[](const std::string& key) {
	return Get(key);
}

template <typename T>
const T* MGLDataMap<T>::operator[](const std::string& key) const {
	return Get(key);
}
