#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "KVec3.h"
#include "KCommon.h"
class Primitive0
{
public:
	Primitive0(int vframeid, KVec3 vpos, KVec3 vrot) :frameid(vframeid), pos(vpos), rot(vrot) {}
	int frameid;
	KVec3 pos, rot;
	void Print();
	string ToJson();
};
class Primitive1 {
public:
	Primitive1() {}
	Primitive1(int vfs, int vfe, KVec3 vposs, KVec3 vpose, KVec3 vrots, KVec3 vrote);
	int frameids, frameide;
	bool bposstatic = false, brotstatic = false;
	KVec3 poss=KVec3(-999,-999,-999), pose = KVec3(-999, -999, -999),rots = KVec3(0, 0, 0),rote = KVec3(0, 0, 0);
	int posfuncid=0, rotfuncid=0;
	vector<float> exvec;

	vector<Primitive0> prim0vec;
	void ToPrimitive0();
	KVec3 GetFunctionalPos(int i);
	KVec3 GetFunctionalRot(int i);
	void Print();
	vector<string> jsonstrvec;
	void ToJson(bool removeHeadTail=true);
	void ToFile(string filepath);
};
class XEntity;
class Primitive2 {
private:
	KVec3 centerpoint=KVec3(-999,-999,-999);
	void _InitDis(XEntity* vcenterentity);
	void _InitDis();
	void _InitRot();
	void _CalculateSizeAndCenterPoint();
public:
	Primitive2() {}
	int shotid;
	string distanceLOD, angleLOD, shotmethod;
	vector<string> mainobjvec;
	vector<int> compositionvec;
	vector<string> techvec;
	float shottime;

	float diss, dise, myfais, myfaie,mythetas,mythetae;
	float perfectfai, perfecttheta, perfectrou;
	float size;
	KVec3 shotpos=KVec3(-999, -999, -999), shotrot=KVec3(-999, -999, -999);
	int segsize = -1;
	vector<vector<KVec3>> dotcloudvec;
	vector<vector<KVec3>> candidatepathvec;
	vector<KVec3> pathvec;

	vector<Primitive0> prim0vec;
	vector<Primitive1> prim1vec;
	vector<string> jsonstrvec;

	void Init();
	KVec3 GetRandomPointAtSeg(int vindex);
	void PrintPath();
	void Print();
	void ToPrimitive0();
	void ToPrimitive1();
	void ToJson(bool removeheadtail=true);
	void ToFile(string filepath);
	void AddToFile(string filepath, bool isfirst = false,bool islast=false);
	KVec3 GetRotToCenterPoint(KVec3 vpos);
	float JudgeComplete(int vcandidateindex);
	float JudgeSmoothness(int vcandidateindex);
	float JudgeFitness(int vcandidateindex);

	void SetPosRotInDotCloud();

	bool _IsExistAndSetPerfect1(XEntity* vobj);
};
#endif

