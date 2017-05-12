#include "XLine.h"
#include "KCommon.h"
#include "XError.h"

XLine::XLine(float va, float vb, float vc) :a(va), b(vb), c(vc) {
	if (NearlyEqualf(b, 0.0f)) {
		b = 0.0f;
	}
	if (NearlyEqualf(a, 0.0f)) {
		a = 0.0f;
	}
	if (NearlyEqualf(c, 0.0f)) {
		c = 0.0f;
	}
}

void XLine::Print() {
	cout << "\nXLine: " << a << "y+(" << b << ")x+(" << c << ")=0";
}

XVec2 XLine::GetTypicalPoint(const XVec2& vp, float voffset) {
	if (NearlyEqualf(b, 0.0f)) {
		if (!NearlyEqualf(vp.y, -c/a)) { throw XError("ERROR:point not at line at XLine::GetTypicalPoint()"); }
		return XVec2(vp.y, vp.x + voffset);
	}
	else {
		float ty = vp.y + voffset;
		return XVec2(ty, -a / b*ty - c / b);
	}
}

XVec2  XLine::GetTypicalPoint() {
	if (NearlyEqualf(b, 0.0f)) {
		return XVec2(-c/a, 1.0f);
	}
	else {
		return XVec2(1.0f, -a / b - c / b);
	}
}

bool XLineIntersction(const XLine& vline1, const XLine& vline2, XVec2& outpoint) {
	float a1 = vline1.a;
	float b1 = vline1.b;
	float c1 = vline1.c;
	float a2 = vline2.a;
	float b2 = vline2.b;
	float c2 = vline2.c;
	float tt = a1*b2 - a2*b1;
	if (NearlyEqualf(tt, 0.0f)) { return false; }
	else {
		outpoint.y = (c2*b1 - c1*b2) / tt;
		outpoint.x = (a2*c1 - a1*c2) / tt;
		return true;
	}
}