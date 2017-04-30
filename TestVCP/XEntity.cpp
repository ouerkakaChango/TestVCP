#include "XEntity.h"
#include "XError.h"
#include <cmath>

XEntity::XEntity(string vname, string vtype, KVec3 vcenterpoint, string vcollisiontype, vector<float> vcollisioninfo) :name(vname), type(vtype), centerpoint(vcenterpoint), collisiontype(vcollisiontype)
, collisioninfo(vcollisioninfo) {
	if (vcollisiontype == "Low_Box"|| vcollisiontype == "High_Box") {
		float x = collisioninfo[0], y = collisioninfo[1], z = collisioninfo[2];
		size=sqrtf(x*x+y*y+z*z);
	}
	else if (vcollisiontype == "Capsule") {
		float r = collisioninfo[0], halfh = collisioninfo[1];
		size = (halfh + r) * 2;
	}
	else {
		throw XError("\n!!!ERROR:CollisionTypeError:" + vcollisiontype);
	}
	XENTITYMGR.entityvec.push_back(this);
}

XEntityManager& XEntityManager::GetInstance() {
	static XEntityManager x;
	return x;
}

XEntity* XEntityManager::Get(string vname) {
	for (auto& i : entityvec) {
		if (i->name == vname) {
			return i;
		}
	}
	return nullptr;
}

#define OPT1(a,b)  (a&&b)
bool XEntityManager::IsLineBeBlocked(float xa, float ya,float za, float xb, float yb,float zb,string vname) {
	//???
	bool re = false;
	for (auto& i : entityvec) {
		if (i != nullptr) {
			if ((i->collisiontype == "Low_Box" || i->collisiontype == "High_Box")&&i->name!=vname) {
				float x1 = i->centerpoint.x - (i->collisioninfo[1]) / 2.0f,
					y1 = i->centerpoint.y - (i->collisioninfo[0]) / 2.0f,
					z1 = i->centerpoint.z - (i->collisioninfo[2]) / 2.0f,
					x2 = i->centerpoint.x + (i->collisioninfo[1]) / 2.0f,
					y2 = i->centerpoint.y + (i->collisioninfo[0]) / 2.0f,
					z2 = i->centerpoint.z + (i->collisioninfo[2]) / 2.0f;
				float xsmall = -1.0f, xbig = -1.0f, ysmall = -1.0f, ybig = -1.0f, zsmall = -1.0f, zbig = -1.0f;
				if (xa < xb) {
					xsmall = xa;
					xbig = xb;
				}
				else {
					xsmall = xb;
					xbig = xa;
				}
				if (ya < yb) {
					ysmall = ya;
					ybig = yb;
				}
				else {
					ysmall = yb;
					ybig = ya;
				}
				if (za < zb) {
					zsmall = za;
					zbig = zb;
				}
				else {
					zsmall = zb;
					zbig = za;
				}
				//optimization1
				/*
				if (
					//opt1
					)
				{
					cout << "\n***" << i->name;
					return true;
				}
				*/
				//else {
					//parameter equation calculate intersection
				if (OPT1(x1 <= xbig,x2 >= xsmall)&&
					OPT1(y1 <= ybig, y2 >= ysmall)&&
					OPT1(z1 <= zbig, z2 >= zsmall)
					) {
					return true;
				}
				//}//Çó´æÔÚµÄelse
			}//if for box
		}//if for i not nullptr
	}//for loop
	return re;
}

bool XEntityManager::IsPointInBlock(KVec3 vp) {
	for (auto& i : entityvec) {
		if (i->collisiontype == "High_Box" || i->collisiontype == "Low_Box") {
			float x1 = i->centerpoint.x - (i->collisioninfo[1]) / 2.0f,
				y1 = i->centerpoint.y - (i->collisioninfo[0]) / 2.0f,
				z1 = i->centerpoint.z - (i->collisioninfo[2]) / 2.0f,
				x2 = i->centerpoint.x + (i->collisioninfo[1]) / 2.0f,
				y2 = i->centerpoint.y + (i->collisioninfo[0]) / 2.0f,
				z2 = i->centerpoint.z + (i->collisioninfo[2]) / 2.0f;
			if (vp.x >= x1&&vp.x <= x2&&vp.y >= y1&&vp.y <= y2&&vp.z >= z1&&vp.z <= z2) {
				return true;
			}
		}
	}
	return false;
}