#include "KCommon.h"
#include <math.h>
#include <sstream>
#include <algorithm>
using std::ostringstream;

KCommon::KCommon()
{
}


KCommon::~KCommon()
{
}

bool NearlyEqualf(float a, float b) {
	if (abs(a - b) <= 0.001) {
		return true;
	}
	return false;
}

string float2string(float n) {
	ostringstream buffer;
	buffer << n;
	return buffer.str();
}

string int2string(int n) {
	ostringstream buffer;
	buffer << n;
	return buffer.str();
}

vector<float> MakeVec(float x, float y, float z) {
	vector<float> re;
	re.push_back(x);
	re.push_back(y);
	re.push_back(z);
	return re;
}

vector<float> MakeVec(float x, float y) {
	vector<float> re;
	re.push_back(x);
	re.push_back(y);
	return re;
}

int IndexOfValue(vector<int>& vec, int value) {
	return std::find(vec.begin(), vec.end(), value) - vec.begin();
}