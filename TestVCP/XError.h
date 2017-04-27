#ifndef XERROR_H
#define XERROR_H
#include "KCommon.h"
class XError{
private:
	string errorinfo;
public:
	XError(string vinfo) :errorinfo(vinfo) {
		cout << errorinfo;
	}
	void PrintError() { cout <<errorinfo; }
};
#endif

