#include <fstream>
#include <iterator>
#include "Primitive.h"
#include "VCPFunction.h"
#include "XEntity.h"
#include "XError.h"
#include "XDataBox.h"
#include "XMath.h"
#include "FFLLAPI.h"	
#include "XLinearConstrainter.h"

void Primitive0::Print() {
	cout << "\nframeid:" << frameid <<"\npos:";
	pos.Print();
	cout << "rot:";
	rot.Print();
}

string Primitive0::ToJson() {
	return "{\"frameid\":" + int2string(frameid) + ",\"pos\":[" + float2string(pos.x) + "," + float2string(pos.y) + "," + float2string((pos.z)) + 
		"],\"rot\":[" + float2string(rot.x) + "," + float2string(rot.y) + "," + float2string(rot.z) + "]}";
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
	if (!removeHeadTail) { jsonstrvec.push_back("["); }
	for(auto iter=prim0vec.begin();iter!=prim0vec.end();iter++){
		string t = "{\"frameid\":" + int2string((*iter).frameid) + ",\"pos\":[" + float2string((*iter).pos.x) + "," + float2string((*iter).pos.y) + "," + float2string((*iter).pos.z) + "],\"rot\":[" +
			float2string((*iter).rot.x) + "," + float2string((*iter).rot.y) + "," + float2string((*iter).rot.z) + "]}";
		jsonstrvec.push_back(t);
	}
	if (!removeHeadTail) { jsonstrvec.push_back("]"); }
}

void Primitive1::ToFile(string filepath) {
	std::ofstream of(filepath);
	for (auto& i : jsonstrvec) {
		of << i;
	}
	of.close();
	cout << "File write ok.\n";
}

void Primitive2::_InitDis(XEntity* vcenterentity) {
	if (distanceLOD == "Far") {
		auto tdis = XDATABOX.GetFar_dis(vcenterentity->collisiontype, vcenterentity->size);
		diss = tdis[0];
		dise = tdis[1];
	}
	else if (distanceLOD == "SmallFull") {
		auto tdis = XDATABOX.GetSmallFull_dis(vcenterentity->collisiontype, vcenterentity->size);
		diss = tdis[0];
		dise = tdis[1];
		perfectrou = vcenterentity->size*XDATABOX.smallfull_dis_capsule_rate[1];
	}
	else {
		throw XError("ERROR:distanceLOD error at Init,is:" + distanceLOD);
	}
}

void Primitive2::_InitRot() {
	if (angleLOD == "Depression") {
		auto tmyfai = XDATABOX.GetDepression_angle(shotmethod);
		myfais = tmyfai[0];
		myfaie = tmyfai[1];
	}
	else if (angleLOD == "Level") {
		auto tmyfai = XDATABOX.GetLevel_myfai(shotmethod);
		myfais = tmyfai[0];
		myfaie = tmyfai[1];
		//perfectfai = 90.0f-XDATABOX.GetLevel_perfectmyfai(shotmethod);

		auto ttheta = XDATABOX.GetLevel_mytheta(shotmethod);
		mythetas = ttheta[0];
		mythetae = ttheta[1];
		//perfecttheta= XDATABOX.GetLevel_perfectmytheta(shotmethod);
	}
	else {
		throw XError("ERROR:angleLOD error at Init,is:" + angleLOD);
	}
}

bool Primitive2::_IsExistAndSetPerfect1(XEntity* vobj) {
	//中心点+距离*正面方向向量
	KVec3 tperfectpoint = centerpoint +(vobj->extroinfovec[0])*perfectrou;

	if (!XENTITYMGR.IsPointInBlock(tperfectpoint)&&!XENTITYMGR.IsLineBeBlocked(tperfectpoint.x,tperfectpoint.y,tperfectpoint.z,centerpoint.x,centerpoint.y,centerpoint.z,vobj->name)) {
		shotpos = tperfectpoint;
		shotrot = GetRotToCenterPoint(tperfectpoint);
		//cout << "\nPerfectPos:"; shotpos.Print();
		//cout << "\nPerfectRot:"; shotrot.Print();
		return true;
	}
	else {
		cout << "\nPerfect false";
		return false;
	}
}

//???
void Primitive2::SetPosRotInDotCloud() {
	if (shotmethod == "Cut") {
		//???
	}
	else {
		throw XError("ERROR:shotmethod error at SetPosRotInDotCloud,is:" + shotmethod);
	}
}

void Primitive2::_CalculateSizeAndCenterPoint() {
	size= (XENTITYMGR.Get(mainobjvec[0])->centerpoint- XENTITYMGR.Get(mainobjvec.back())->centerpoint).GetLength();
	KVec3 tcen(0.0f, 0.0f, 0.0f);
	for (auto& i : mainobjvec) {
		tcen += XENTITYMGR.Get(i)->centerpoint;
	}
	tcen /= static_cast<float>((int)mainobjvec.size());
	centerpoint = tcen;
}

void Primitive2::Init() {
	if (mainobjvec.size() == 1) {
		auto pobj = XENTITYMGR.Get(mainobjvec[0]);
		if (pobj!=nullptr) {
			//确定中心点
			centerpoint = pobj->centerpoint;
			if (shotmethod == "Surround") {
				//确定diss等4个变量
				_InitDis(pobj);
				_InitRot();
				//diss = 3000;
				//dise = 3100;
				//angles = 30;
				//anglee = 48;
				//现在已有可行解范围了，生成点云
				cout << "\n4v: " << diss << " " << dise << " " << myfais << " " << myfaie;
				//离散和分段数据
				//???
				float deltafai = 3.0f, deltatheta = 20.0f, deltarou = 20.0f;
				                           //2
				segsize = 360/12;//6段
				int candidatesize = 5; //20次随机

				//初始化
				for (int i = 0; i < 360/segsize; i++) {
					dotcloudvec.push_back(vector<KVec3>());
				}
				for (int i = 0; i < candidatesize; i++) {
					candidatepathvec.push_back(vector<KVec3>());
				}
				//球面fai(90-anglee,90-angles),theta(0,2*pi),rou(diss,dise)
				for (float fai = 90.0f - myfaie; fai <= 90.0f - myfais; fai += deltafai) {
					for (float theta = 0.0f; theta < 360.0f; theta += deltatheta) {
						for (float rou = diss; rou <= dise; rou += deltafai) {
							//碰撞检测去除点
							//???非常耗时，能否优化
							KVec3 tpoint = SCToCCoordinate(fai, theta, rou) + pobj->centerpoint;
							if (!XENTITYMGR.IsPointInBlock(tpoint)) {
								//点云
								//dotcloudvec.push_back(SCToCCoordinate(fai, theta, rou));
								//根据theta分段放入各个"区域"
								int t1 = static_cast<int>(theta);
								int index = (t1 - t1 % segsize) / segsize;
								dotcloudvec[index].push_back(tpoint);
							}
							else{}
						}
					}
				}//fai的for循环
				//cout <<"\n"<<dotcloudvec[0].capacity();

				float tmaxgoodness = 0.0f;
				int tbesti = 0;
				for (int cani = 0; cani < candidatesize; cani++) {
					//从每个区域随机出一个点然后相连，生成路径
					for (int i = 0; i < (int)dotcloudvec.size(); i++) {
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
					float tgoodness = 0.6f*tjcomplete + 0.3f*(NearlyEqualf(tjsmoothness, 0.0f) ? 1.0f : atanf(1 / tjsmoothness)) + 0.1f*tjfitness / (16.0f*candidatepathvec[0].size());
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
			else if (shotmethod == "Cut") {
				//确定diss等6个变量
				_InitDis(pobj);
				_InitRot();
				//由于是1个人/物体，所以不用考虑兴趣线（只取人物正面）
				//首先寻找最优角度距离的点
				//如果存在并可见，则赋值shotpos,shotrot,
				if (_IsExistAndSetPerfect1(pobj)) {

				}
				else {
					//如果不存在，则生成点云，多次随机并取（可见性，适中性）的最优点
					//由6个参数生成点云，去掉碰撞点
					KVec3 tsccenter = CCToSCoordinate(centerpoint);
					float tcentertheta = tsccenter.y;
					float deltafai = 2.0f, deltatheta = 2.0f, deltarou = 10.0f;
					bool istwosides = false;
					dotcloudvec.push_back(vector<KVec3>());//dotcloud[0]
					if (tcentertheta<mythetae || tcentertheta>360.0f - mythetae) {
						istwosides = true;
					}
					if (!istwosides) {
						for (float fai = 90.0f - myfaie; fai <= 90.0f - myfais; fai += deltafai) {
							for (float theta = tcentertheta + mythetas; theta <= tcentertheta + mythetae; theta += deltatheta) {
								for (float rou = diss; rou <= dise; rou += deltarou) {
									KVec3 tpoint = SCToCCoordinate(fai, theta, rou) + pobj->centerpoint;
									if (!XENTITYMGR.IsPointInBlock(tpoint)) {
										//点云
										dotcloudvec[0].push_back(tpoint);
									}
								}
							}
						}
					}
					else {
						//分两批
						float thetas1 = -1.0f, thetae1 = 360.0f, thetas2 = 0.0f, thetae2 = -1.0f;
						if (tcentertheta > 270.0f) {
							thetas1 = tcentertheta + mythetas;
							thetae2 = tcentertheta + mythetae - 360.0f;
						}
						else {
							thetas1 = tcentertheta + 360.0f + mythetas;
							thetae2 = tcentertheta + mythetae;
						}
						for (float fai = 90.0f - myfaie; fai <= 90.0f - myfais; fai += deltafai) {
							for (float theta = thetas1; theta <= thetae1; theta += deltatheta) {
								for (float rou = diss; rou <= dise; rou += deltarou) {
									KVec3 tpoint = SCToCCoordinate(fai, theta, rou) + pobj->centerpoint;
									if (!XENTITYMGR.IsPointInBlock(tpoint)) {
										//点云
										dotcloudvec[0].push_back(tpoint);
									}
								}
							}
						}
						for (float fai = 90.0f - myfaie; fai <= 90.0f - myfais; fai += deltafai) {
							for (float theta = thetas2; theta <= thetae2; theta += deltatheta) {
								for (float rou = diss; rou <= dise; rou += deltarou) {
									KVec3 tpoint = SCToCCoordinate(fai, theta, rou) + pobj->centerpoint;
									if (!XENTITYMGR.IsPointInBlock(tpoint)) {
										//点云
										dotcloudvec[0].push_back(tpoint);
									}
								}
							}
						}
					}//else for istwosides

					cout << "\ngg";
					cout << dotcloudvec[0].size();

					SetPosRotInDotCloud();

				}//else for 如果不存在 perfect
			}//如果是cut
			else {
				throw XError("ERROR:shotmethod error at init,is:" + shotmethod);
			}

		}//物体非空
		else {
			throw XError("!!!EntityGetError:nullptr at Init(),Name:" + mainobjvec[0]);
		}
	}//1个物体
	else if (mainobjvec.size() >= 2) {
		if (shotmethod == "Cut") {
			int tsize = (int)mainobjvec.size();
			_CalculateSizeAndCenterPoint();
			//???
			//获得约束线集合（1,2,3条）
			//1.初始化所有直线
			XLinearConstrainter tconter;
			for (int i = 0; i < tsize; i++) {
				auto tobj1 = XENTITYMGR.Get(mainobjvec[IndexOfValue(compositionvec, i)]);
				float ty1 = tobj1->centerpoint.y;
				float tx1 = tobj1->centerpoint.x;
				tconter.pointvec.push_back(XVec2(ty1, tx1));
			}
			for (int i = 0; i < tsize - 1; i++) {
				auto tobj1 = XENTITYMGR.Get(mainobjvec[ IndexOfValue(compositionvec,i)]);
				float y1 = tobj1->centerpoint.y;
				float x1 = tobj1->centerpoint.x;
				for (int j = i + 1; j < tsize; j++) {
					auto tobj2 = XENTITYMGR.Get(mainobjvec[IndexOfValue(compositionvec, j)]);
					float y2 = tobj2->centerpoint.y;
					float x2 = tobj2->centerpoint.x;
					tconter.PushLine(y1,x1,y2,x2,i,j);
				}
			}
			//2.简化
			tconter.Simplify();
			//确定圆心，计算rou范围，theta范围,rou范围由databox中调整得来
			//即InitDis,Rot()...
			XPRINT(tconter);
		}
		else {
			throw XError("ERROR:shotmethod error at init,is:" + shotmethod);
		}
	}//多个物体
	else {
		throw XError("ERROR:0 object in prim2");
	}//无物体
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
		int frames = (int)XDATABOX.nowtime*XDATABOX.framesize;
		int framee = frames + (int)shottime*XDATABOX.framesize;
		frames += 1;
		XDATABOX.nowtime += shottime;
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
	else if (shotmethod == "Cut") {
		int frames= (int)XDATABOX.nowtime * XDATABOX.framesize;
		int framee= frames + (int)shottime*XDATABOX.framesize;
		frames += 1;
		XDATABOX.nowtime += shottime;
		//cout << "\n nowtimes:" << XDATABOX.nowtime<<"\n";
		//cout << "\n framee:" << framee;
		for (int id = frames; id <= framee; id++) {
			prim0vec.push_back(Primitive0(id, shotpos, shotrot));
		}
	}
	else {
		throw XError("ERROR:ShotMethodError At P2ToP0,ShotMethod:"+shotmethod);
	}
}

//???类型转换的帧，帧间隔的不整除???
void Primitive2::ToPrimitive1() {
	if (shotmethod == "Surround") {
		int frames =(int) XDATABOX.nowtime * XDATABOX.framesize;
		int framee = frames + (int)shottime*XDATABOX.framesize;
		frames += 1;
		XDATABOX.nowtime += shottime;
		//cout << "\n" << XDATABOX.nowtime << "\n";
		int numofseg = 360 / segsize;//12
		float tdeltaframe = static_cast<float>(framee - frames) / numofseg;
		int tnowframe = frames;
		//将1,n-1个1层原语放入
		               //13-2=11
		int i = 0;
		KVec3 tp1=KVec3(-999,-999,-999), tp2 = KVec3(-999, -999, -999);
		for (i=0;i<(int)(pathvec.size()-2);i++){
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

//演示中突然旋转360度是因为-359到0插帧的问题
KVec3 Primitive2::GetRotToCenterPoint(KVec3 vpos) {
	float x = vpos.x-centerpoint.x, y = vpos.y-centerpoint.y, z = vpos.z-centerpoint.z;
	float yaw=-1.0f;	
	//cout << "\nAfter cut" << x << " " << y << " " << z<<"\n";
	/*
		if (x >= 0.0f&&y > 0.0f) {
			yaw = -ArcToDegree(atanf(x / y));
			cout << "\ny1";
		}
		else if (y < 0.0f) {
			yaw = -180.0f - ArcToDegree(atanf(x / y));
			cout << "\ny2";
		}
		else if (x<0.0f&&y>0.0f) {
			yaw = -360.0f - ArcToDegree(atanf(x / y));
			cout << "\ny3";
		}
		else if (y = 0.0f) {
			if (x >= 0.0f) {
				yaw = -90.0f;
			}
			else {
				yaw = -180.0f;
			}
		}
		yaw += 270.0f;*/
	/*
	x>0, y <= 0:atan(y / x)[0, -90]
		x<0, y <= 0 : -90 - atan(y / x)[-90, -180]
		x<0, y>0 : -180 + atan(y / x)
		x>0, y>0:-360 + atan(y / x)
		*/
	if (x < 0.0f&&y>=0.0f) {
		yaw = atanf(y / x);
		//cout << "\ny1";
	}
	else if(x>0.0f&&y>=0.0f){
		yaw = -90.0f - atanf(y / x);
		//cout << "\ny2";
	}
	else if (x>0.0f&&y<0.0f) {
		yaw = -180.0f + atanf(y / x);
		//cout << "\ny3";
	}
	else if (x < 0.0f&&y < 0.0f) {
		yaw = -360.0f + atanf(y / x);
		//cout << "\ny4";
	}
	else if(x=0.0f){
		if (y >= 0.0f) {
			yaw = -90.0f;
		}
		else {
			yaw = -270.0f;
		}
	}
	return KVec3(-90.0f+ArcToDegree(acosf(z / sqrtf(x*x + y*y + z*z))), yaw,  0.0f);
}

void Primitive2::Print() {
	for (auto& i : jsonstrvec) {
		cout << i << "\n";
	}
}

void Primitive2::ToJson(bool removeheadtail) {
	if (shotmethod == "Surround") {
		jsonstrvec.clear();
		if (!removeheadtail) { jsonstrvec.push_back("["); }
		for (auto p1 : prim1vec) {
			p1.ToPrimitive0();
			p1.ToJson(true);
			std::copy(p1.jsonstrvec.begin(), p1.jsonstrvec.end(), std::back_inserter(jsonstrvec));
		}
		//jsonstrvec[jsonstrvec.size() - 1].erase(jsonstrvec[jsonstrvec.size() - 1].end() - 1);
		if (!removeheadtail) { jsonstrvec.push_back("]"); }
	}
	else if (shotmethod == "Cut") {
		for (auto& i : prim0vec) {
			jsonstrvec.push_back(i.ToJson());
		}
	}
	else {
		throw XError("ERROR:shotmethod at ToJson,is:" + shotmethod);
	}
}

void Primitive2::ToFile(string filepath) {
	std::ofstream of(filepath);
	for (auto& i : jsonstrvec) {
		of << i;
	}
	of.close();
	cout << "File write ok.\n";
}

void Primitive2::AddToFile(string filepath, bool isfirst, bool islast) {
	std::ofstream outfile;
	outfile.open(filepath, std::ios::app);
	if (!outfile) //检查文件是否正常打开
	{
		throw XError("ERROR:file cannot open at AddToFile(),is:" + filepath);
	}
	else
	{
		if (isfirst) { outfile << "["; }
		for (auto iter = jsonstrvec.begin(); iter != jsonstrvec.end(); iter++) {
			if (islast&&iter == jsonstrvec.end() - 1) {
				outfile << *iter;
			}
			else {
				outfile << ((*iter) + ",");
			}
		}
		if (islast) { outfile << "]"; }
		outfile.close();
	}
}