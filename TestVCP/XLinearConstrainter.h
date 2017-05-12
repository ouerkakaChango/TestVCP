#ifndef XLINEARCONSTRAINTER_H
#define XLINEARCONSTRAINTER_H
#include "XLine.h"
#include "KCommon.h"
#include "XVec2.h"
#include <tuple>

#define XIDLine std::tuple<XLine,int,int,XVec2>
class XLinearConstrainter{
public:
	vector<XIDLine> constraintvec;
	vector<XVec2> pointvec;
	void PushLine(float y1, float x1, float y2, float x2,int vid1,int vid2);
	void Simplify();
	void Print();
	bool IsExist(int i,int j);
	bool IsLineSame(const XIDLine& vidline, int vid1,int vid2);
	bool LineIntersction(const XIDLine& vline1, const XIDLine& vline2, XVec2& outpoint);
	bool IsNeedReserve(int i, int j, int k, const XIDLine& vidline);
};
#endif
