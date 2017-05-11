#ifndef XLINE_H
#define XLINE_H
//x=k*y+t
class XLine{
public:
	float a, b, c;
	XLine(float va, float vb, float vc) :a(va), b(vb), c(vc) {}
	void Print();
};
#endif

