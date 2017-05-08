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

vector<float> XDataBox::GetSmallFull_dis(string vcollisiontype, float vsize) {
	if (vcollisiontype == "Capsule") {
		return MakeVec(vsize*smallfull_dis_capsule_rate[0], vsize*smallfull_dis_capsule_rate[2]);
	}
	else {
		throw XError("!!!Error:CollisionTypeError At GetSmallFar_dis,CollisionType:" + vcollisiontype);
		return vector<float>();
	}
}

vector<float> XDataBox::GetLevel_myfai(string vshotmethod) {
	if (vshotmethod == "Cut") {
		return MakeVec(level_myfai[0], level_myfai[2]);
	}
	else {
		throw XError("!!!Error:ShotMethodError At GetLevel_myfai,ShotMethod:" + vshotmethod);
		return vector<float>();
	}
}

vector<float> XDataBox::GetLevel_mytheta(string vshotmethod) {
	if (vshotmethod == "Cut") {
		return MakeVec(level_mytheta[0], level_mytheta[2]);
	}
	else {
		throw XError("!!!Error:ShotMethodError At GetLevel_mytheta,ShotMethod:" + vshotmethod);
		return vector<float>();
	}
}

float XDataBox::GetLevel_perfectmyfai(string vshotmethod) {
	if (vshotmethod == "Cut") {
		return level_myfai[1];
	}
	else {
		throw XError("!!!Error:ShotMethodError At GetLevel_perfectmyfai,ShotMethod:" + vshotmethod);
		return -1.0f;
	}
}

float XDataBox::GetLevel_perfectmytheta(string vshotmethod) {
	if (vshotmethod == "Cut") {
		return level_mytheta[1];
	}
	else {
		throw XError("!!!Error:ShotMethodError At GetLevel_perfectmytheta,ShotMethod:" + vshotmethod);
		return -1.0f;
	}
}

