#include "stdafx.h"

#include "MGLInputItem.h"

MGLInputItem::MGLInputItem(const GLuint k, const GLuint a1, const GLuint i, const GLuint m, void* funcData) :
	keyValue(k),
	action(a1),
	index(i),
	keyMod(m),
	functionData(funcData) {}

MGLInputItem::~MGLInputItem() {
	//if (functionData && deleteFuncData) 
		//delete functionData;
}

MGLInputItem::MGLInputItem(const MGLInputItem&& other) noexcept {
	keyValue = other.keyValue;
	action = other.action;
	index = other.index;
	keyMod = other.keyMod;
	inUse = other.inUse;
	functionData = other.functionData; // funcdata is only refrenced, not managed in MGLInputItem....
}
