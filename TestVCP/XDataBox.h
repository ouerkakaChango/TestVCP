#ifndef XDATABOX_H
#define XDATABOX_H
#include "KCommon.h"

class XDataBox{
public:
	int framesize = 60;
	static XDataBox& GetInstance();
	vector<float> depression_angle = MakeVec(50.0f, 90.0f);
	vector<float> level_myfai = MakeVec(-20.0f, 20.0f);
	vector<float> level_mytheta = MakeVec(-30.0f, 30.0f);
	vector<float> far_dis_lowbox_rate = MakeVec(1.05f, 2.16f, 3.27f);
	vector<float> smallfull_dis_capsule_rate = MakeVec(1.14f, 1.43f, 2.14f);
	float surround_anglemax = 80.0f;

	vector<float> GetFar_dis(string vcollisiontype,float vsize);
	vector<float> GetDepression_angle(string vshotmethod);
	vector<float> GetSmallFull_dis(string vcollisiontype, float vsize);
	vector<float> GetLevel_myfai(string vshotmethod);
	vector<float> GetLevel_mytheta(string vshotmethod);
};
#define XDATABOX XDataBox::GetInstance()
#endif
