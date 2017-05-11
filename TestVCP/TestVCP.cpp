#include <iostream>
#include "Primitive.h"
#include "XEntity.h"
using namespace std;

int main()
{
	XEntity e_ground("Ground", "StaticMesh", KVec3(0, 0, 20), "Low_Box", MakeVec(1000.0f, 1000.0f, 50.0f));

	XEntity e_cha1("Cha1", "Character", KVec3(200, 200, 130), "Capsule", MakeVec(30.0f, 40.0f));
	e_cha1.extroinfovec.push_back(KVec3(-1.0f,0.0f,0.0f));
	XEntity cha2("Cha2", "Character", KVec3(200, -400, 130), "Capsule", MakeVec(30.0f, 40.0f));
	XEntity cha3("Cha3", "Character", KVec3(0, -400, 130), "Capsule", MakeVec(30.0f, 40.0f));

	XEntity e_box1("BlockBox1", "StaticMesh", KVec3(0, -1250, 3000), "Low_Box", MakeVec(200.0f, 200.0f, 200.0f));
	XEntity e_box2("BlockBox2", "StaticMesh", KVec3(0, 1000, 3000), "Low_Box", MakeVec(200.0f, 200.0f, 200.0f));
	vector<Primitive2> shotvec;
	shotvec.push_back(Primitive2());
	shotvec[0].shotid = 1;
	shotvec[0].distanceLOD = "Far";
	shotvec[0].angleLOD = "Depression";
	shotvec[0].shotmethod = "Surround";
	shotvec[0].mainobjvec.push_back("Ground");
	shotvec[0].shottime = 5.0f;
	//shotvec[0].Init();
	//shotvec[0].ToPrimitive1();
	//shotvec[0].ToJson();
	
	shotvec.push_back(Primitive2());
	shotvec[1].shotid = 2;
	shotvec[1].distanceLOD = "SmallFull";
	shotvec[1].angleLOD = "Level";
	shotvec[1].shotmethod = "Cut";
	shotvec[1].mainobjvec.push_back("Cha1");
	shotvec[1].shottime = 3.0f;
	//shotvec[1].Init();
	//shotvec[1].ToPrimitive0();
	//shotvec[1].ToJson();

	shotvec.push_back(Primitive2());
	shotvec[2].shotid = 3;
	shotvec[2].distanceLOD = "SmallFull";
	shotvec[2].angleLOD = "Level";
	shotvec[2].shotmethod = "Cut";
	shotvec[2].mainobjvec.push_back("Cha2");
	shotvec[2].mainobjvec.push_back("Cha3");
	shotvec[2].shottime = 3.0f;
	shotvec[2].Init();

	/*
	for (auto iter = shotvec.begin(); iter != shotvec.end(); iter++) {
		if (iter == shotvec.begin()) {
			iter->AddToFile("D:/TestVCPJson.txt", true);
		}
		else if(iter==shotvec.end()-1){
			iter->AddToFile("D:/TestVCPJson.txt",false,true);
		}
		else {
			iter->AddToFile("D:/TestVCPJson.txt");
		}
	}
	*/
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

