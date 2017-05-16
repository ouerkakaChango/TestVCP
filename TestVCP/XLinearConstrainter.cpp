#include "XLinearConstrainter.h"
#include "XError.h"
#include "XMath.h"
void XLinearConstrainter::PushLine(float y1, float x1, float y2, float x2, int vid1, int vid2) {
	float a = x2 - x1, b = y1 - y2, c = y2*x1 - y1*x2;
	/*
	bool tgt = true;
	if (NearlyEqualf(b, 0.0f)) {
		//默认b为0时a不可能为0,a>0
		b = 0.0f;
		tgt = x1 < x2;
		if (a < 0) { tgt = !tgt; }//a<0
	}
	else {
		//只要斜率存在,b>0
		tgt = y1 > y2;
		if (b < 0) { tgt = !tgt; }//b<0
	}
	*/
	XVec2 tvec(y2-y1,x2-x1);
	tvec.Normallize();
	constraintvec.push_back(XIDLine(XLine(a, b, c), vid1, vid2,tvec));
}

bool XLinearConstrainter::IsLineSame(const XIDLine& vidline, int vid1, int vid2) {
	int tid1 = std::get<1>(vidline), tid2 = std::get<2>(vidline);
	return tid1 == vid1&&tid2 == vid2;
}

bool XLinearConstrainter::IsExist(int i, int j) {
	if (i >= j) {throw XError("\nERROR:i>=j at XLinearConstrainter::IsExist.");}
	for (auto& iter : constraintvec) {
		if (IsLineSame(iter,i,j)) {
			return true;
		}
	}
	return false;
}
bool XLinearConstrainter::LineIntersction(const XIDLine& vline1,const XIDLine& vline2, XVec2& outpoint) {
	//判断两线是否相交，如相交就把结果存入outpoint
	int line1i = std::get<1>(vline1);
	int line1j = std::get<2>(vline1);
	int line2i = std::get<1>(vline2);
	int line2j = std::get<2>(vline2);
	if (line1i == line2i || line1i == line2j ) {
		outpoint = pointvec[line1i];
		return true;
	}
	else if (line1j == line2i || line1j == line2j) {
		outpoint = pointvec[line1j];
		return true;
	}
	else {
		return XLineIntersction(std::get<0>(vline1), std::get<0>(vline2), outpoint);
	}
}

XIDLine XLinearConstrainter::GetLine(int i, int j) {
	for (auto& iter : constraintvec) {
		if (std::get<1>(iter) == i&&std::get<2>(iter) == j) {
			return iter;
		}
	}
	throw XError("\nError:Line not found at XLinearConstrainter::GetLine()");
}

bool XLinearConstrainter::IsNeedReserve(int i, int j, int k, const XIDLine& vidline, int vmode) {
	//判断(i,j)(j,k)的射线对与直线vidline有无交点
	//默认不存在两线为一条线的情况
	XVec2 ta(-99.0f, -99.0f), tb(-99.0f, -99.0f);
	XVec2 p = XVec2(-99.0f, -99.0f);
	XIDLine linea=std::make_tuple(XLine(-99.0f, -99.0f, -99.0f),-1,-1,XVec2(-99.0f,-99.0f)), lineb = std::make_tuple(XLine(-99.0f, -99.0f, -99.0f), -1, -1, XVec2(-99.0f, -99.0f));
	if (vmode==1) {
		linea = GetLine(i, j), lineb = GetLine(j, k);
		p = pointvec[j];
	}
	else if(vmode==2){
		linea = GetLine(i, k), lineb = GetLine(j, k);
		p = pointvec[k];
	}
	else if (vmode == 3) {
		linea = GetLine(i, j), lineb = GetLine(i, k);
		p = pointvec[i];
	}
	else {
		throw XError("\nERROR:vmode error at XLinearConstrainter::IsNeedReserve,is:" + int2string(vmode));
	}
	bool ista = LineIntersction(linea, vidline, ta), istb = LineIntersction(lineb, vidline, tb);
	
	if (!ista&&istb) {
		//判断(j, tb)与A.Vec的叉积，正就删
		if (Cross(XVec2(tb - p), std::get<3>(linea)) > 0) {
			return false;
		}
	}
	else if (ista && !istb) {
		//判断(j, ta)与B.Vec的叉积，正就删
		if (Cross(XVec2(ta - p), std::get<3>(lineb)) > 0) {
			return false;
		}
	}
	else if (!ista && !istb) {
		throw XError("\nERROR:Both not intersect at XLinearConstrainter::IsNeedReserve()");
	}
	else if (ista&&istb) {
		if (NearlyEqual(ta,tb)&&NearlyEqual(tb,p)){
			//在vidline上j点处 + 1 - 1，看是否存在满足，都不满足就删
			//XPRINT(ta);
			//XPRINT(tb);
			//XPRINT(pj);
			//cout << "\n>>>i:" << i << " j:" << j << " k:" << k;
			//cout << "\n linec:" << std::get<1>(vidline)<< std::get<2>(vidline);
				XLine tlinec = std::get<0>(vidline);
				XVec2 tcp1 = tlinec.GetTypicalPoint(p, 1.0f),
					tcp2 = tlinec.GetTypicalPoint(p, -1.0f);
				bool tb1 = IsSatisfy(linea, lineb, tcp1),
					tb2 = IsSatisfy(linea, lineb, tcp2);
				if ((!tb1) && (!tb2)) {
					return false;
				}
	    }
		else {
			//if (i == 0 && j == 1 && k == 2 && t1 == 0 && t2 == 2) { cout << "\n***"; }
			bool is1 = IsSameDir(XVec2(ta - p), std::get<3>(linea));// (j, ta)与A.Vec同向？
			bool is2 = IsSameDir(XVec2(p-tb), std::get<3>(lineb)); //(tb, j)与B.Vec同向？
				if (
					(is1&&is2) || 
					((!is1) && (!is2)))
				{
					if (!IsSatisfy(linea,lineb,(ta+tb)/2.0f)) { //ta,tb中点
						return false;
					}
					else {
					}
				}
		}

	}
	return true;
}

void XLinearConstrainter::Simplify() {
	//将不需要的直线除去
	//1.取i,j=i+1,k=j+1的三点组成的两线(i,j)(j,k),获取射线对。若(i,j)或(j,k)已不在constraintvec中，则continue;
	//2.遍历constraintvec中其他非(i,j)(j,k)的线，看与射线对若无交点，则从constraintvec中去除
	cout << "\nSimplify Start";
	int tsize = constraintvec.size();
	for (int i = 0; i < tsize - 2; i++) {
		for (int j = i + 1; j < tsize - 1; j++) {
			for (int k = j + 1; k < tsize; k++) {
				///////////
				//mode1:j
				if (IsExist(i, j) && IsExist(j, k)) {
					for (auto iter = constraintvec.begin(); iter != constraintvec.end();) {
						if (IsLineSame(*iter, i, j) || IsLineSame(*iter, j, k)) {
							iter++;
							continue;
						}
						else {
							if (IsNeedReserve(i, j, k, *iter,1)) {
								//保留
								iter++;
							}
							else {
								iter=constraintvec.erase(iter);
							}
						}
					}//for for contraintvec
				}
				////////
				//mode2:k
				//if (i == 0 && j == 2 && k == 3) { cout << "\n***"; }
				if (IsExist(i, k) && IsExist(j, k)) {
					for (auto iter = constraintvec.begin(); iter != constraintvec.end();) {
						if (IsLineSame(*iter, i, k) || IsLineSame(*iter, j, k)) {
							iter++;
							continue;
						}
						else {
							if (IsNeedReserve(i, j, k, *iter,2)) {
								//保留
								iter++;
							}
							else {
								iter = constraintvec.erase(iter);
							}
						}
					}//for for contraintvec
				}
				////////
				//mode3:i
				if (IsExist(i, j) && IsExist(i, k)) {
					for (auto iter = constraintvec.begin(); iter != constraintvec.end();) {
						if (IsLineSame(*iter, i, j) || IsLineSame(*iter, i, k)) {
							iter++;
							continue;
						}
						else {
							if (IsNeedReserve(i, j, k, *iter, 3)) {
								//保留
								iter++;
							}
							else {
								iter = constraintvec.erase(iter);
							}
						}
					}//for for contraintvec
				}
				////////
			}//for for k
		}
	}
	cout << "\nSimplify Done";
}

XVec2 XLinearConstrainter::GetCirclePoint(float vangle) {
	if (constraintvec.size() == 1) {
		XIDLine& tlineref = constraintvec[0];
		XVec2 ta = pointvec[std::get<1>(tlineref)];
		XVec2 tb = pointvec[std::get<2>(tlineref)];
		XVec2 tv1 = tb - ta;
		return ta + tv1.Rotate(vangle);
	}
	else if (constraintvec.size() == 2) {
		//cout << "\n222line circlepoint";
		XVec2 re(-99.0f,-99.0f);
		if (LineIntersction(constraintvec[0], constraintvec[1], re)) {
			return re;
		}
		else {
			throw XError("\nERROR:At XLinearConstrainter::GetCirclePoint(),2line donnot have intersection");
		}
	}
	else if (constraintvec.size() == 3) {
		//cout << "\n333line circlepoint";
		XVec2 re(-99.0f, -99.0f);
		if (LineIntersction(constraintvec[0], constraintvec[2], re)) {
			return re;
		}
		else {
			throw XError("\nERROR:At XLinearConstrainter::GetCirclePoint(),2line donnot have intersection");
		}
	}
	else {
		throw XError("\nERROR:At XLinearConstrainter::GetCirclePoint()");
	}
}

float XLinearConstrainter::GetAB(XVec2& outa, XVec2& outb) {
	if (constraintvec.size() == 1) {
		XIDLine& tlineref = constraintvec[0];
		outa = pointvec[std::get<1>(tlineref)];
		outb = pointvec[std::get<2>(tlineref)];
	}
	else {
		throw XError("\nERROR:At XLinearConstrainter::GetAB()");
	}
}

float XLinearConstrainter::GetEXRou(float vmythetae) {
	if (constraintvec.size() == 1) {
		XIDLine& tlineref = constraintvec[0];
		XVec2 tveca = pointvec[std::get<1>(tlineref)];
		XVec2 tvecb = pointvec[std::get<2>(tlineref)];
		return (tvecb - tveca).GetLength() / 2.0f / sinf(DegreeToArc(vmythetae));
	}
	else if (constraintvec.size() == 2) {
		return 0.0f;
	}
	else if (constraintvec.size() == 3) {
		//???不精确
		XVec2 t1(-99.0f, -99.0f);
		if (LineIntersction(constraintvec[0], constraintvec[1], t1)) {}
		else {
			throw XError("\nERROR:At XLinearConstrainter::GetEXRou,line donnot have intersection");
		}
		XVec2 t2(-99.0f, -99.0f);
		if (LineIntersction(constraintvec[0], constraintvec[2], t2)) {}
		else {
			throw XError("\nERROR:At XLinearConstrainter::GetEXRou,line donnot have intersection");
		}
		return (t2 - t1).GetLength();
	}
	else {
		throw XError("\nERROR:At XLinearConstrainter::GetEXRou");
	}
}

KVec3 XLinearConstrainter::GetForward(const XVec2& vcirclepoint) {
	if (constraintvec.size() == 1) {
		XIDLine& tlineref = constraintvec[0];
		XVec2 tveca = pointvec[std::get<1>(tlineref)];
		XVec2 tvecb = pointvec[std::get<2>(tlineref)];
		return KVec3((tveca + tvecb) / 2.0f - vcirclepoint, 0.0f);
	}
	else if(constraintvec.size() == 2){
		XLine& l1ref = std::get<0>(constraintvec[0]);
		XLine& l2ref = std::get<0>(constraintvec[1]);
		float a1 = l1ref.a, b1 = l1ref.b, c1 = l1ref.c;
		float a2 = l2ref.a, b2 = l2ref.b, c2 = l2ref.c;
		float ld = sqrtf((a1*a1 + b1*b1) / (a2*a2 + b2*b2));
		XLine ttestl1(a1+ld*a2,b1+ld*b2,c1+ld*c2);
		XVec2 tp1 = ttestl1.GetTypicalPoint(vcirclepoint, 10.0f);
		if (IsSatisfy(constraintvec[0], constraintvec[1], tp1)) {
			return KVec3(tp1 - vcirclepoint, 0.0f);
		}
		XVec2 tp2 = ttestl1.GetTypicalPoint(vcirclepoint, -10.0f);
		if (IsSatisfy(constraintvec[0], constraintvec[1], tp2)) {
			return KVec3(tp2 - vcirclepoint, 0.0f);
		}
		XLine ttestl2(a1 - ld*a2, b1 - ld*b2, c1 - ld*c2);
		XVec2 tp3 = ttestl2.GetTypicalPoint(vcirclepoint, 10.0f);
		if (IsSatisfy(constraintvec[0], constraintvec[1], tp3)) {
			return KVec3(tp3 - vcirclepoint, 0.0f);
		}
		XVec2 tp4 = ttestl2.GetTypicalPoint(vcirclepoint, -10.0f);
		if (IsSatisfy(constraintvec[0], constraintvec[1], tp4)) {
			return KVec3(tp4 - vcirclepoint, 0.0f);
		}
		throw XError("\nERROR:At XLinearConstrainter::GetForwardVec,cannot get forward in size=2");
	}
	else if (constraintvec.size() == 3) {
		XLine& l1ref = std::get<0>(constraintvec[0]);
		XLine& l2ref = std::get<0>(constraintvec[2]);
		float a1 = l1ref.a, b1 = l1ref.b, c1 = l1ref.c;
		float a2 = l2ref.a, b2 = l2ref.b, c2 = l2ref.c;
		float ld = sqrtf((a1*a1 + b1*b1) / (a2*a2 + b2*b2));
		XLine ttestl1(a1 + ld*a2, b1 + ld*b2, c1 + ld*c2);
		XVec2 tp1 = ttestl1.GetTypicalPoint(vcirclepoint, 10.0f);
		if (IsSatisfy(constraintvec[0], constraintvec[2], tp1)) {
			return KVec3(tp1 - vcirclepoint, 0.0f);
		}
		XVec2 tp2 = ttestl1.GetTypicalPoint(vcirclepoint, -10.0f);
		if (IsSatisfy(constraintvec[0], constraintvec[2], tp2)) {
			return KVec3(tp2 - vcirclepoint, 0.0f);
		}
		XLine ttestl2(a1 - ld*a2, b1 - ld*b2, c1 - ld*c2);
		XVec2 tp3 = ttestl2.GetTypicalPoint(vcirclepoint, 10.0f);
		if (IsSatisfy(constraintvec[0], constraintvec[2], tp3)) {
			return KVec3(tp3 - vcirclepoint, 0.0f);
		}
		XVec2 tp4 = ttestl2.GetTypicalPoint(vcirclepoint, -10.0f);
		if (IsSatisfy(constraintvec[0], constraintvec[2], tp4)) {
			return KVec3(tp4 - vcirclepoint, 0.0f);
		}
		throw XError("\nERROR:At XLinearConstrainter::GetForwardVec,cannot get forward in size=2");
	}
	else {
		throw XError("\nERROR:At XLinearConstrainter::GetForwardVec");
	}
}

void XLinearConstrainter::Print() {
	for (auto& i : constraintvec) {
		std::get<0>(i).Print();
		cout << " id1:" << std::get<1>(i) << " id2:" << std::get<2>(i);
	    std::get<3>(i).Print();
	}
}


bool IsSatisfy(const XIDLine& vlinea, const XIDLine& vlineb, const XVec2& vtestpoint) {
	XLine tl1 = std::get<0>(vlinea);
	XLine tl2 = std::get<0>(vlineb);
	XVec2 tv1= std::get<3>(vlinea);
	XVec2 tv2 = std::get<3>(vlineb);
	XVec2 m1 = tl1.GetTypicalPoint();
	XVec2 m2 = tl2.GetTypicalPoint();
	/*
	XPRINT(vtestpoint);
	cout << "\nlinea:" << std::get<1>(vlinea) << std::get<2>(vlinea);
	cout << "\nlineb:" << std::get<1>(vlineb) << std::get<2>(vlineb);
	XPRINT(tv1);
	XPRINT(tv2);
	auto tt = vtestpoint - m1;
	cout<<"\n???"<<Cross(XVec2(-1,0), XVec2(0,1));
	*/
	return (Cross(tv1, vtestpoint - m1) > 0) && (Cross(tv2, vtestpoint - m2) > 0);
}