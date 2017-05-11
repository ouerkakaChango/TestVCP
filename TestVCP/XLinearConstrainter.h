#ifndef XLINEARCONSTRAINTER_H
#define XLINEARCONSTRAINTER_H
#include "XLine.h"
#include "KCommon.h"
#include <tuple>

#define XIDLine std::tuple<XLine,int,int>
class XLinearConstrainter{
public:
	vector<XIDLine > constraintvec;
	void PushLine(float y1, float x1, float y2, float x2,int vid1,int vid2);
	void Simplify();
	void Print();
};
#endif
