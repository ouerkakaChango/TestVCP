#include "XVec2.h"
#include "XMath.h"
#include "XError.h"
float XVec2::GetLength() {
	return sqrtf(y*y + x*x);
}

void XVec2::Normallize() {
	if (NearlyEqualf(y, 0.0f) && NearlyEqualf(x, 0.0f)) {
		y = 0.0f;
		x = 0.0f;
		cout << "\n!!!WARNING:XVec2(0,0)";
	}
	else {
		float tl = GetLength();
		y /= tl;
		x /= tl;
	}
}

XVec2 XVec2::operator-() {
	return XVec2(-y, -x);
}

XVec2 XVec2::operator-(const XVec2& v) const{
	return XVec2(y - v.y, x - v.x);
}

XVec2 XVec2::operator+(XVec2& v) {
	return XVec2(y + v.y, x + v.x);
}

XVec2 XVec2::operator/(float n) {
	return XVec2(y / n, x / n);
}

void XVec2::Print() {
	cout << "\nvec2:y=" << y << ",x=" << x;
}

float Cross(const XVec2& v1, const XVec2& v2) {
	//x1y2-x2y1
	return v1.y*v2.x - v2.y*v1.x;
}

bool NearlyEqual(const XVec2& v1, const XVec2& v2) {
	if (NearlyEqualf(v1.y, v2.y) && NearlyEqualf(v1.x, v2.x)) {
		return true;
	}
	else {
		return false;
	}
}

bool IsSameDir(XVec2 v1,XVec2 v2) {
	v1.Normallize();
	v2.Normallize();
	if (NearlyEqual(v1, v2)) { return true; }
	else if (NearlyEqual(v1, -v2) ){ return false; }
	else {
		throw XError("\nERROR:NotOneLineVec in IsSameDir");
	}
}