#ifndef XLINE_H
#define XLINE_H
//x=k*y+t
class XLine{
public:
	float k, t,A;
	bool isA=false;
	XLine(float vk, float vt) :k(vk),t(vt),isA(false) {}
	XLine(float vA) :A(vA),isA(true) {}
	void Print();
};
#endif

