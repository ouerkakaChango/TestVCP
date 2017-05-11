#ifndef KCOMMON_H
#define KCOMMON_H
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
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
int IndexOfValue(vector<int>& vec, int value);
#define K_PI 3.14159265358979323846f
#define XPRINT(v) do{ \
cout<<"\n"<<#v<<" ";\
v.Print();\
}while (0)
#define XFIND(vec,value) std::find(vec.begin(),vec.end(),value)!=vec.end()
#endif

