#include "XLinearConstrainter.h"
#include "XError.h"
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

bool XLinearConstrainter::IsNeedReserve(int i, int j, int k, const XIDLine& vidline) {
	//判断(i,j)(j,k)的射线对与直线vidline有无交点
	//默认不存在两线为一条线的情况
	XVec2 ta(-99.0f, -99.0f), tb(-99.0f, -99.0f);
	XIDLine linea = GetLine(i, j), lineb = GetLine(j, k);
	bool ista = LineIntersction(linea, vidline, ta), istb = LineIntersction(lineb, vidline, tb);
	XVec2 pk = pointvec[k];
	if (!ista&&istb) {
		//判断(k, tb)与A.Vec的叉积，正就删
		if (Cross(XVec2(tb - pk), std::get<3>(linea)) > 0) {
			return false;
		}
	}
	else if (ista && !istb) {
		//判断(k, ta)与B.Vec的叉积，正就删
		if (Cross(XVec2(ta - pk), std::get<3>(lineb)) > 0) {
			return false;
		}
	}
	else if (!ista && !istb) {
		throw XError("\nERROR:Both not intersect at XLinearConstrainter::IsNeedReserve()");
	}
	else if (ista&&istb) {
		if (NearlyEqual(ta,tb)&&NearlyEqual(tb,pk)){
			//在vidline上k点处 + 1 - 1，看是否存在满足，都不满足就删
				XLine tlinec = std::get<0>(vidline);
				XVec2 tcp1 = tlinec.GetTypicalPoint(pk, 1.0f),
					tcp2 = tlinec.GetTypicalPoint(pk, -1.0f);
				bool tb1 = IsSatisfy(linea, lineb, tcp1),
					tb2 = IsSatisfy(linea, lineb, tcp2);
				if ((!tb1) && (!tb2)) {
					return false;
				}
	    }
		else {
			bool is1 = IsSameDir(XVec2(ta - pk), std::get<3>(linea));// (k, ta)与A.Vec同向？
			bool is2 = IsSameDir(XVec2(pk-tb), std::get<3>(linea)); //(tb, k)与B.Vec同向？
				if (
					(is1&&is2) || 
					((!is1) && (!is2)))
				{
					if (!IsSatisfy(linea,lineb,(ta+tb)/2.0f)) { //ta,tb中点
						return false;
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
				if (IsExist(i, j) && IsExist(j, k)) {
					for (auto iter = constraintvec.begin(); iter != constraintvec.end();) {
						if (IsLineSame(*iter, i, j) || IsLineSame(*iter, i, j)) {
							iter++;
							continue;
						}
						else {
							if (IsNeedReserve(i, j, k, *iter)) {
								//保留
								iter++;
							}
							else {
								iter=constraintvec.erase(iter);
							}
						}
					}//for
				}
				else {
					continue;
				}
			}
		}
	}
	cout << "\nSimplify Done";
}

XVec2 XLinearConstrainter::GetCirclePoint() {
	if (constraintvec.size() == 1) {
		XIDLine& tlineref = constraintvec[0];
		XVec2 ta = pointvec[std::get<1>(tlineref)];
		XVec2 tb = pointvec[std::get<2>(tlineref)];
		XVec2 tv1 = tb - ta;
		return ta + tv1.Rotate(60.0f);
	}
	else if (constraintvec.size() == 2) {

	}
	else if (constraintvec.size() == 3) {

	}
	else {
		throw XError("\nERROR:At XLinearConstrainter::GetCirclePoint()");
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
	return (Cross(tv1, vtestpoint - m1) > 0) && (Cross(tv2, vtestpoint - m2) > 0);
}