#include "stdafx.h"
#include "MGLInputItem.h"

MGLInputItem::MGLInputItem(GLuint k, GLuint a1, GLuint a2, GLuint i, GLuint m, void* funcData) {
	keyValue = k; 
	action1 = a1;
	action2 = a2; 
	index = i;
	keyMod = m; 
	functionData = funcData;
}

GLboolean MGLInputItem::KeyShouldRun() {
	return false;
}