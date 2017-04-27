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
				float x1 = i->centerpoint.x - (i->collisioninfo[1]) / 2.0,
					y1 = i->centerpoint.y - (i->collisioninfo[0]) / 2.0,
					z1 = i->centerpoint.z - (i->collisioninfo[2]) / 2.0,
					x2 = i->centerpoint.x + (i->collisioninfo[1]) / 2.0,
					y2 = i->centerpoint.y + (i->collisioninfo[0]) / 2.0,
					z2 = i->centerpoint.z + (i->collisioninfo[2]) / 2.0;
				//optimization1
				if (
					OPT1(xa<x1, xb<x1) ||
					OPT1(xa>x2, xb>x2) ||
					OPT1(ya<y1, yb<y1) ||
					OPT1(ya>y2, yb>y2) ||
					OPT1(ya<y1, yb<y1) ||
					OPT1(ya>y2, yb>y2) ||
					OPT1(za<z1, zb<z1) ||
					OPT1(za>z2, zb>z2) ||
					OPT1(xa<x1, xb<x1) ||
					OPT1(xa>y2, xb>y2) ||
					OPT1(za<z1, zb<z1) ||
					OPT1(za>z2, zb>z2)
					)
				{
					cout << "\n***" << i->name;
					return true;
				}
				else {
					//parameter equation calculate intersection
					float m = xb - xa,
						n = yb - ya,
						t = zb - za;
					float tx=xa, ty=ya, tz=za;
					for (float tdelta = 0.0f; xa + tdelta <= xb; tdelta += 0.1f) {
						tx += m*tdelta;
						ty += n*tdelta;
						tz += t*tdelta;
						//check if the point in box
						if (tx >= x1&&tx <= x2&&ty >= y1&&ty <= y2&&tz >= z1&&tz <= z2) {
							//cout << "\n***" << i->name<<" "<<vname;
							return true;
						}
					}
					//???
				}
			}//if for box
		}//if for i not nullptr
	}//for loop
	return re;
}