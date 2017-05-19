#include "KVec3.h"
#include "KCommon.h"
#include "XMath.h"
#include "XError.h"
#include <math.h>

KVec3::KVec3(float vx, float vy, float vz) :x(vx), y(vy), z(vz) {
	if (NearlyEqualf(x, 0.0f)) {
		x = 0.0f;
	}
	if (NearlyEqualf(y, 0.0f)) {
		y = 0.0f;
	}
	if (NearlyEqualf(z, 0.0f)) {
		z = 0.0f;
	}
}

void KVec3::SetNormal() {
	float length = GetLength();
	x /= length;
	y /= length;
	z /= length;
}

float KVec3::GetLength() {
	float re = sqrtf(x*x + y*y + z*z);
	//if (NearlyEqualf(re, 0.0f)){ abort(); return 0.0f; }
	return re;
}

float KVec3::Dot(KVec3 v) {
	return x*v.x + y*v.y + z*v.z;
}

float KVec3::Cos(KVec3 v) {
	return Dot(v) / (GetLength()*v.GetLength());
}

KVec3  KVec3::Cross(KVec3 v) {
	return KVec3(y*v.z - v.y*z, v.x*z - x*v.z, x*v.y - v.x*y);
}

KVec3 KVec3::operator-(KVec3& v) {
	return KVec3(x - v.x, y - v.y, z - v.z);
}

KVec3 KVec3::operator+(KVec3& v) {
	return KVec3(x + v.x, y + v.y, z + v.z);
}

void  KVec3::operator+=(KVec3 v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

KVec3 KVec3::operator/(float n) {
	return KVec3(x / n, y / n, z / n);
}

void KVec3::operator/=(float n) {
	if (NearlyEqualf(n, 0.0f)) {
		throw XError("\nError:0 at operator/=");
	}
	else {
		x /= n;
		y /= n;
		z /= n;
	}
}

KVec3 KVec3::operator*(float n) {
	return KVec3(x*n, y*n, z*n);
}

bool KVec3::NearlyEqual(KVec3 v) {
	if (NearlyEqualf(x, v.x) && NearlyEqualf(y, v.y) && NearlyEqualf(z, v.z)) {
		return true;
	}
	return false;
}

bool KVec3::NearlyZero() {
	return NearlyEqual(KVec3(0.0f, 0.0f, 0.0f));
}

float KVec3::GetLineDis(KVec3 v, KVec3 vab) {
	//a=Dot(vecab,n)/Length(n)
	KVec3 n = this->Cross(v);
	if (n.NearlyZero()) {
		//parallel
		//d=Length(Cross(vab,v))/v.GetLength()
		return Cross(vab).GetLength() / v.GetLength();
	}
	else {
		return vab.Dot(n) / n.GetLength();
	}
}

float KVec3::GetAngleBetween(KVec3 vn) {
	return 90.0f - ArcToDegree( acos(this->Dot(vn)/vn.GetLength()/this->GetLength()));
}

void KVec3::Print() {
	cout << "vec3:" << x << " " << y << " " << z << "\n";
}

void KVec3::RotatePitch(float vpitch) {
		KVec3 tsc = CCToSCoordinate(x,y,z);
		tsc.x -= vpitch;
		//cout << "\n***" << vpitch;
		tsc= SCToCCoordinate(tsc.x, tsc.y, tsc.z);
		x = tsc.x; y = tsc.y; z = tsc.z;
}