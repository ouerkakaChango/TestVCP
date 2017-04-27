#include <iostream>
#include "Primitive.h"
#include "XEntity.h"
using namespace std;

int main()
{
	XEntity e_ground("Ground", "StaticMesh", KVec3(0, 0, 20), "Low_Box", MakeVec(1000, 1000, 50));
	XEntity e_cha1("Cha1", "Character", KVec3(200, 200, 20), "Capsule", MakeVec(30, 40));
	Primitive2 p2;
	p2.shotid = 1;
	p2.distanceLOD = "Far";
	p2.angleLOD = "Depression";
	p2.shotmethod = "Surround";
	p2.mainobjvec.push_back("Ground");
	p2.shottime = 5.0f;
	p2.Init();
	//p2.ToPrimitive1();
	//cout << "\n***" << p2.prim1vec.size();
	//p2.ToJson();
	//p2.Print();
	//p2.ToPrimitive0();
	//p2.ToFile("D:/TestVCPJson.txt");
	/*
	Primitive1 p1;
	p1.frameids = 1;
	p1.frameide = 300;
	p1.poss = KVec3(0, 0, 200);
	p1.pose = KVec3(-400, 0, 50);
	p1.posfuncid = 0;
	p1.rote = KVec3(0, 0, -45);
	p1.rotfuncid = 0;
	*/
	//p1.ToPrimitive0();
	//p1.ToJson();
	//p1.Print();
	//p1.ToFile("D:/TestVCPJson.txt");
	system("pause");
    return 0;
}

