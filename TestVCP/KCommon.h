#ifndef KCOMMON_H
#define KCOMMON_H
#include <vector>
#include <iostream>
#include <string>
using std::vector;
using std::cout;
using std::string;
class KCommon
{
public:
	KCommon();
	~KCommon();
};
bool NearlyEqualf(float a, float b);
string float2string(float n);
string int2string(int n);
vector<float> MakeVec(float x, float y, float z);
vector<float> MakeVec(float x, float y);
#define K_PI 3.14159265358979323846f
#endif

