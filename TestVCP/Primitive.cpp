#include <fstream>
#include <iterator>
#include "Primitive.h"
#include "VCPFunction.h"
#include "XEntity.h"
#include "XError.h"
#include "XDataBox.h"
#include "XMath.h"
#include "FFLLAPI.h"	

void Primitive0::Print() {
	cout << "\nframeid:" << frameid <<"\npos:";
	pos.Print();
	cout << "rot:";
	rot.Print();
}

Primitive1::Primitive1(int vfs, int vfe, KVec3 vposs, KVec3 vpose, KVec3 vrots, KVec3 vrote):frameids(vfs),frameide(vfe),poss(vposs),pose(vpose),rots(vrots),rote(vrote),
bposstatic(false),brotstatic(false),posfuncid(0),rotfuncid(0)
{
}

void Primitive1::ToPrimitive0() {
	KVec3 tpos(-999,-999,-999), trot(-999,-999,-999);
	for (int i = frameids; i <= frameide; i++) {
		if (bposstatic) {
			tpos = poss;
		}
		else {
			tpos = GetFunctionalPos(i);
		}
		if (brotstatic) {
			trot = rots;
		}
		else {
			trot = GetFunctionalRot(i);
		}
		prim0vec.push_back(Primitive0(i, tpos, trot));
	}
}

KVec3 Primitive1::GetFunctionalPos(int i) {
	return FUNCMGR.Func(poss, pose, posfuncid, frameide - frameids, i - frameids);
}

KVec3 Primitive1::GetFunctionalRot(int i) {
	return FUNCMGR.Func(rots, rote, rotfuncid, frameide - frameids, i - frameids);
}

void Primitive1::Print() {
	for (auto& i :jsonstrvec) {
		cout << i<<"\n";
	}
}

void Primitive1::ToJson(bool removeHeadTail) {
	jsonstrvec.clear();
	if (removeHeadTail) {}
	else { jsonstrvec.push_back("["); }
	for(auto iter=prim0vec.begin();iter!=prim0vec.end();iter++){
		string t = "{\"frameid\":" + int2string((*iter).frameid) + ",\"pos\":[" + float2string((*iter).pos.x) + "," + float2string((*iter).pos.y) + "," + float2string((*iter).pos.z) + "],\"rot\":[" +
			float2string((*iter).rot.x) + "," + float2string((*iter).rot.y) + "," + float2string((*iter).rot.z) + "]}";
		if (removeHeadTail) { t += ","; }
		else {
			if (iter != prim0vec.end() - 1) { t += ","; }
		}
		jsonstrvec.push_back(t);
	}
	if (removeHeadTail) {}
	else { jsonstrvec.push_back("]"); }
}

void Primitive1::ToFile(string filepath) {
	std::ofstream of(filepath);
	for (auto& i : jsonstrvec) {
		of << i;
	}
	of.close();
	cout << "File write ok.\n";
}

void Primitive2::Init() {
	if (mainobjvec.size() == 1) {
		auto pobj = XENTITYMGR.Get(mainobjvec[0]);
		if (pobj!=nullptr) {
			//确定中心点
			centerpoint = pobj->centerpoint;
			if (shotmethod == "Surround") {
				//确定diss等4个变量
				auto tdis = XDATABOX.GetFar_dis(pobj->collisiontype, pobj->size);
				auto tangle = XDATABOX.GetDepression_angle(shotmethod);
				diss = tdis[0];
				dise = tdis[1];
				angles = tangle[0];
				anglee = tangle[1];
				diss = 3000;
				dise = 3100;
				//angles = 30;
				//anglee = 48;
				//现在已有可行解范围了，生成点云
				cout << "\n4v: " << diss << " " << dise << " " << angles << " " << anglee;
				//离散和分段数据
				//???
				float deltafai = 3.0f, deltatheta = 2.0f, deltarou = 20.0f;
				segsize = 360/6;//6段
				int candidatesize = 5;

				//初始化
				for (int i = 0; i < 360/segsize; i++) {
					dotcloudvec.push_back(vector<KVec3>());
				}
				for (int i = 0; i < candidatesize; i++) {
					candidatepathvec.push_back(vector<KVec3>());
				}
				//球面fai(90-anglee,90-angles),theta(0,2*pi),rou(diss,dise)
				for (float fai = 90.0f - anglee; fai <= 90.0f - angles; fai += deltafai) {
					for (float theta = 0.0f; theta < 360.0f; theta += deltatheta) {
						for (float rou = diss; rou <= dise; rou += deltafai) {
							//???碰撞检测去除点
							//点云
							//dotcloudvec.push_back(SCToCCoordinate(fai, theta, rou));
							//根据theta分段放入各个"区域"
							int t1 = static_cast<int>(theta);
							int index = (t1 - t1 % segsize) / segsize;
							dotcloudvec[index].push_back(SCToCCoordinate(fai, theta, rou)+pobj->centerpoint);
						}
					}
				}//fai的for循环
				//cout <<"\n"<<dotcloudvec[0].capacity();

				float tmaxgoodness = 0.0f;
				int tbesti = 0;
				for (int cani = 0; cani < candidatesize; cani++) {
					//从每个区域随机出一个点然后相连，生成路径
					for (int i = 0; i < dotcloudvec.size(); i++) {
						candidatepathvec[cani].push_back(GetRandomPointAtSeg(i));
					}
					//candidatepathvec[cani].push_back(candidatepathvec[cani][0]);
					//评价路径，此时模糊逻辑和（平滑性，适中性,完整率）上场
					float tjcomplete = JudgeComplete(cani);
					cout << "\nCOMPLETE:" << tjcomplete;
					float tjsmoothness = JudgeSmoothness(cani);
					cout << "\nSMOOTHNESS:" << tjsmoothness;
					float tjfitness = JudgeFitness(cani);
					cout << "\nFITNESS:" << tjfitness;
					//???
					//取权值计算最优值
					//(complete:0.6,smoothness:0.3,fitness:0.1)
					float tgoodness = 0.6f*tjcomplete + 0.3f*(NearlyEqualf(tjsmoothness, 0.0f) ? 1.0f : atan(1 / tjsmoothness)) + 0.1f*tjfitness / (16.0*candidatepathvec[0].size());
					cout << "\n!!!Final goodness:" << tgoodness;
					if (tgoodness > tmaxgoodness) {
						tbesti = cani;
						tmaxgoodness = tgoodness;
					}
					//cout << "\n111:" << 0.6*tjcomplete;
					//cout << "\n222:" << 0.3*(NearlyEqualf(tjsmoothness, 0.0f) ? 1.0f : atan(1 / tjsmoothness));
					//cout << "\n333:" << ;
				}
				pathvec = candidatepathvec[tbesti];
				//PrintPath();
				//cout << "\n@@@" << prim0vec.size();

			}//###"surround"的if

		}//物体非空
		else {
			throw XError("!!!EntityGetError:Name:" + mainobjvec[0]);
		}
	}//1个物体
}

float Primitive2::JudgeComplete(int vcandidateindex) {
	int totalnum = candidatepathvec[vcandidateindex].size() - 1;
	int cansee = 0;
	for (int i = 0; i < totalnum; i++) {
		if (!XENTITYMGR.IsLineBeBlocked(candidatepathvec[vcandidateindex][i].x, candidatepathvec[vcandidateindex][i].y, candidatepathvec[vcandidateindex][i].z,
			centerpoint.x, centerpoint.y,centerpoint.z, XENTITYMGR.Get(mainobjvec[0])->name)) {
			cansee++;
		}
	}
	return static_cast<float>(cansee) / static_cast<float>(totalnum);
}

float Primitive2::JudgeSmoothness(int vcandidateindex) {
	float re = -1.0f;
	int totalnum = candidatepathvec[vcandidateindex].size() - 1;
	vector<float> anglevec;
	for (int i=0; i < totalnum; i++) {
		KVec3 t1 = candidatepathvec[vcandidateindex][i], t2 = candidatepathvec[vcandidateindex][i + 1];
		anglevec.push_back(t1.GetAngleBetween(t2));
	}
	return GetStandardDeviation(anglevec);
}

#define DIS	0 // our health is 1st variable
#define ANG	1 // enemy health is 2nd variable

//??? 0 4 16
float Primitive2::JudgeFitness(int vcandidateindex) {
	float re = 0.0f;
	if (shotmethod == "Surround") {
		for (auto& i : candidatepathvec[vcandidateindex]) {
			KVec3 tcc = i - centerpoint;
			float trou = tcc.GetLength() / (XENTITYMGR.Get(mainobjvec[0])->size)*100.0f;
			float tfai = 90.0f - ArcToDegree(acos(tcc.z / tcc.GetLength()));
			//cout << "\nOriginal Dis:" << trou << " Ang:" << tfai;
			int model = ffll_new_model();//

			int ret_val = (int)ffll_load_fcl_file(model, "fltest1.fcl");//

			if (ret_val < 0)
			{
				// make sure the "working directory" in "Project | Settings..."
				// is set to the executable's directory if running from the MSVC IDE
				throw XError("Error Opening aiwisdom.fcl");
			}
			int child = ffll_new_child(model);//
			ffll_set_value(model, child, DIS, trou); //
			ffll_set_value(model, child, ANG, tfai); //

			int output = (int)ffll_get_output_value(model, child); //
			//cout << "\n!!!:" << ffll_get_output_value(model, child);
			switch (output)
			{
			case (1):
				//cout << "\nNot good";
				break;

			case (2):
				//cout << "\nSosoGood";
				re += 4.0f;
				break;

			case (3):
				//cout << "\nVeryGood";
				re += 16.0f;
				break;
			default:
				throw XError("\nFuzzyReturnError");
				break;
			} // end switch

		}
	}
	else {
		throw XError("\nShotMethodError At JudgeFitness(),SHOTMETHOD:" + shotmethod);
	}
	return re;
}

KVec3 Primitive2::GetRandomPointAtSeg(int vindex) {
		std::uniform_int_distribution<unsigned> u(0, dotcloudvec[vindex].size() - 1);
		return dotcloudvec[vindex][u(XMATH.random_engine)];
}

void Primitive2::PrintPath(){
	for (auto& i : pathvec) {
		i.Print();
	}
}

void Primitive2::ToPrimitive0() {
	if (shotmethod == "Surround") {
		int frames = (shotid - 1)*XDATABOX.framesize;
		int framee = frames + (int)shottime*XDATABOX.framesize;
		int numofseg = 360 / segsize;//12
		float tdeltaframe = static_cast<float>(framee - frames) / numofseg;
		int tcount = 0;
		int tindex = 0;
		for (int id = frames; id <= framee; id++) {
			prim0vec.push_back(Primitive0(id, pathvec[tindex], GetRotToCenterPoint(pathvec[tindex])));
			tcount++;
			if (tcount >= tdeltaframe) {
				tcount = 0;
				tindex++;
			}
		}
	}
	else {
		throw XError("ERROR:ShotMethodError At P2ToP0,ShotMethod:"+shotmethod);
	}
}

//???类型转换的帧，帧间隔的不整除???
void Primitive2::ToPrimitive1() {
	if (shotmethod == "Surround") {
		int frames = (shotid - 1)*XDATABOX.framesize;
		int framee = frames + (int)shottime*XDATABOX.framesize;
		int numofseg = 360 / segsize;//12
		float tdeltaframe = static_cast<float>(framee - frames) / numofseg;
		int tnowframe = frames;
		//将1,n-1个1层原语放入
		               //13-2=11
		int i = 0;
		KVec3 tp1=KVec3(-999,-999,-999), tp2 = KVec3(-999, -999, -999);
		for (i=0;i<pathvec.size()-2;i++){
			tp1= pathvec[i], tp2= pathvec[i + 1];
			prim1vec.push_back(Primitive1(tnowframe, tnowframe + (int)tdeltaframe, tp1, tp2, GetRotToCenterPoint(tp1), GetRotToCenterPoint(tp2)));
			tnowframe += (int)tdeltaframe + 1;
		}
		tp1 = pathvec[pathvec.size() - 2], tp2 = pathvec[pathvec.size() - 1];
		prim1vec.push_back(Primitive1(tnowframe, framee, tp1, tp2, GetRotToCenterPoint(tp1), GetRotToCenterPoint(tp2)));
	}
	else {
		throw XError("ERROR:ShotMethodError At P2ToP0,ShotMethod:" + shotmethod);
	}
}

KVec3 Primitive2::GetRotToCenterPoint(KVec3 vpos) {
	float x = vpos.x-centerpoint.x, y = vpos.y-centerpoint.y, z = vpos.z-centerpoint.z;
	float yaw=-1.0f;	
		if (x >= 0.0f&&y > 0.0f) {
			yaw = -ArcToDegree(atanf(x / y));
		}
		else if (y < 0.0f) {
			yaw = -180.0f - ArcToDegree(atanf(x / y));
		}
		else if (x<0.0f&&y>0.0f) {
			yaw = -360.0f - ArcToDegree(atanf(x / y));
		}
		yaw += 270.0f;
	return KVec3(-90.0f+ArcToDegree(acosf(z / sqrtf(x*x + y*y + z*z))), yaw,  0.0f);
}

void Primitive2::Print() {
	for (auto& i : jsonstrvec) {
		cout << i << "\n";
	}
}

void Primitive2::ToJson() {
	jsonstrvec.clear();
	jsonstrvec.push_back("[");
	for (auto p1 : prim1vec) {
		p1.ToPrimitive0();
		p1.ToJson(true);
		std::copy(p1.jsonstrvec.begin(), p1.jsonstrvec.end(), std::back_inserter(jsonstrvec));
	}
	jsonstrvec[jsonstrvec.size() - 1].erase(jsonstrvec[jsonstrvec.size() - 1].end() - 1);
	jsonstrvec.push_back("]");
}

void Primitive2::ToFile(string filepath) {
	std::ofstream of(filepath);
	for (auto& i : jsonstrvec) {
		of << i;
	}
	of.close();
	cout << "File write ok.\n";
}