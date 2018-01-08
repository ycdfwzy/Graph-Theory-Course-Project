#include "Utility.h"

const char* heap_str(const string& str) {
	char* res = new char[str.length() + 1];
	strcpy_s(res, str.length() + 1, str.c_str());
	return res;
}
