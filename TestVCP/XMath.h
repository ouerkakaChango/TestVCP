#ifndef XMATH_H
#define XMATH_H
#include "KVec3.h"
#include "KCommon.h"
#include <random>

class XMath{
public:
	std::default_random_engine random_engine;
	static XMath& GetInstance();
};
#define XMATH XMath::GetInstance()
KVec3 SCToCCoordinate(float fai, float theta, float rou);
KVec3 CCToSCoordinate(float x, float y, float z);
KVec3 CCToSCoordinate(KVec3 v);
float ArcToDegree(float arc);
float GetStandardDeviation(vector<float> vec);
#endif
