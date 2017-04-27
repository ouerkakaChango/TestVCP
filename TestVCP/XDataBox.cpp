#include "XDataBox.h"
#include "XError.h"
XDataBox& XDataBox::GetInstance() {
	static XDataBox x;
	return x;
}

vector<float> XDataBox::GetFar_dis(string vcollisiontype, float vsize) {
	if (vcollisiontype == "Low_Box") {
		return MakeVec(vsize*far_dis_lowbox_rate[0], vsize*far_dis_lowbox_rate[2]);
	}
	else {
		throw XError("!!!Error:CollisionTypeError At GetFar_dis,CollisionType:" + vcollisiontype);
		return vector<float>();
	}
}

vector<float> XDataBox::GetDepression_angle(string vshotmethod) {
	if (vshotmethod == "Surround") {
		return MakeVec(depression_angle[0],surround_anglemax);
	}
	else {
		throw XError("!!!Error:ShotMethodError At GetDepression_angle,ShotMethod:" + vshotmethod);
		return vector<float>();
	}
}

