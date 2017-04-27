#ifndef XDATABOX_H
#define XDATABOX_H
#include "KCommon.h"

class XDataBox{
public:
	int framesize = 60;
	static XDataBox& GetInstance();
	vector<float> depression_angle = MakeVec(50.0f, 90.0f);
	vector<float> far_dis_lowbox_rate = MakeVec(1.05f, 2.16f, 3.27f);
	float surround_anglemax = 80.0f;

	vector<float> GetFar_dis(string vcollisiontype,float vsize);
	vector<float> GetDepression_angle(string vshotmethod);
};
#define XDATABOX XDataBox::GetInstance()
#endif
