#ifndef XLINE_H
#define XLINE_H
#include "XVec2.h"
class XLine{
public:
	float a, b, c;
	XLine(float va, float vb, float vc) ;
	void Print();
	XVec2 GetTypicalPoint(const XVec2& vp,float voffset);
	XVec2 GetTypicalPoint();
};

bool XLineIntersction(const XLine& vline1, const XLine& vline2, XVec2& outpoint);
#endif

