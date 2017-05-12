#ifndef XVEC2_H
#define XVEC2_H
class XVec2
{
public:
	float y,x;
	XVec2(float vy, float vx) :y(vy), x(vx) {}
	float GetLength();
	void Normallize();
	friend float Cross(const XVec2& v1, const XVec2& v2);
	friend bool IsSameDir(XVec2 v1, XVec2 v2);
	XVec2 operator-();
	XVec2 operator- (const XVec2& v) const;
	XVec2 operator+(XVec2& v);
	XVec2 operator/(float n);
	void Print();
};
float Cross(const XVec2& v1, const XVec2& v2);
bool IsSameDir( XVec2 v1,  XVec2 v2);
bool NearlyEqual(const XVec2& v1, const XVec2& v2);
#endif

