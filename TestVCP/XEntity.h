#ifndef XENTITY_H
#define XENTITY_H
#include "KCommon.h"
#include "KVec3.h"
#include <tuple>

#define XMotion std::tuple<KVec3,KVec3>
class XEntity {
public:
	string name,type,collisiontype,motionfile;
	KVec3 centerpoint;
	float size;//to calculate
	vector<float> collisioninfo;
	vector<KVec3> extroinfovec;
	
	vector<XMotion> motionvec;
	XEntity(string vname, string vtype, KVec3 vcenterpoint, string vcollisiontype, vector<float> vcollisioninfo);
	void InitMotionFile(string vfile);
};

class XEntityManager {
public:
	vector<XEntity*> entityvec;
	static XEntityManager& GetInstance();
	XEntity* Get(string vname);
	bool IsLineBeBlocked(float xa, float ya, float za,float xb, float yb,float zb,string vname);
	bool IsPointInBlock(KVec3 vp);
};
#define XENTITYMGR XEntityManager::GetInstance()
#endif
