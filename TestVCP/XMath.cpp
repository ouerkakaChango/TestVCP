#include "XMath.h"
#include "KCommon.h"
#include <cmath>

XMath& XMath::GetInstance() {
	static XMath x;
	return x;
}

KVec3 SCToCCoordinate(float fai, float theta, float rou) {
	float x, y, z;
	fai = fai / 180.0f*K_PI;
	theta = theta / 180.0f*K_PI;
	y = rou*sinf(fai)*cosf(theta);
	x = rou*sinf(fai)*sinf(theta);
	z = rou*cosf(fai);
	return KVec3(x, y, z);
}

KVec3 CCToSCoordinate(float x, float y, float z) {
	float fai = -1.0f, theta = -1.0f, rou = -1.0f;
	fai = ArcToDegree(atanf(x / y));
	rou = sqrtf(x*x + y*y + z*z);
	theta = ArcToDegree(acosf(z / rou));
	return KVec3(fai, theta, rou);
}

KVec3 CCToSCoordinate(KVec3 v) {
	return CCToSCoordinate(v.x, v.y, v.z);
}

float ArcToDegree(float arc) {
	return arc / 2.0f / K_PI*360.0f;
}

float GetStandardDeviation(vector<float> vec) {
	float ave = 0.0f;
	for (auto i : vec) {
		ave += i;
	}
	ave /= vec.size();
	float re = 0.0f;
	for (auto i : vec) {
		re += powf(i - ave, 2);
	}
	re /= vec.size();
	re = sqrtf(re);
	return re;
}