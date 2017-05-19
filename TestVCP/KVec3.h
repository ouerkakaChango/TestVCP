#ifndef KVEC3_H
#define KVEC3_H
#include "XVec2.h"
class KVec3 {
public:
	float x, y, z;
	KVec3(float vx, float vy, float vz);
	KVec3(XVec2 v2, float vz) :x(v2.x), y(v2.y), z(vz) {}
	void SetNormal();
	float GetLength();
	float Dot(KVec3 v);
	float Cos(KVec3 v);
	KVec3 Cross(KVec3 v);
	KVec3 operator-(KVec3& v);
	KVec3 operator+(KVec3& v);
	KVec3 operator/(float n);
	KVec3 operator*(float n);
	void operator+=(KVec3 v);
	void operator/=(float n);
	float GetLineDis(KVec3 v, KVec3 vab);
	float GetAngleBetween(KVec3 vn);
	bool NearlyEqual(KVec3 v);
	bool NearlyZero();
	void Print();
	void RotatePitch(float vpitch);
};
#endif
