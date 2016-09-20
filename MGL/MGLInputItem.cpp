#include "stdafx.h"

#include "MGLInputItem.h"

MGLInputItem::MGLInputItem(const GLuint k, const GLuint a1, const GLuint a2, const GLuint i, const GLuint m, void* funcData) :
	keyValue(k),
	action1(a1),
	action2(a2),
	index(i),
	keyMod(m),
	functionData(funcData) {}

MGLInputItem::MGLInputItem(const MGLInputItem&& other) noexcept {
	keyValue = other.keyValue;
	action1 = other.action1;
	action2 = other.action2;
	index = other.index;
	keyMod = other.keyMod;
	inUse = other.inUse;
	functionData = other.functionData; // funcdata is only refrenced, not managed in MGLInputItem....
}
