#include "XLinearConstrainter.h"
#include "XError.h"
void XLinearConstrainter::PushLine(float y1, float x1, float y2, float x2, int vid1, int vid2) {
	float a = x2 - x1, b = y1 - y2, c = y2*x1 - y1*x2;
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
	constraintvec.push_back(XIDLine(XLine(a, b, c), vid1, vid2,tgt));
}

bool XLinearConstrainter::IsLineSame(XIDLine& vidline, int vid1, int vid2) {
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

bool XLinearConstrainter::IsHaveIntersection(int i, int j, int k, XIDLine& vidline) {
	//???
	//判断(i,j)(j,k)的射线对与直线vidline有无交点
	//符合两条线的点满足两个关系式，求存在问题
	//默认不存在两线为一条线的情况
	return false;
}

void XLinearConstrainter::Simplify() {
	//???
	//将不需要的直线除去
	//1.取i,j=i+1,k=j+1的三点组成的两线(i,j)(j,k),获取射线对。若(i,j)或(j,k)已不在constraintvec中，则continue;
	//2.遍历constraintvec中其他非(i,j)(j,k)的线，看与射线对若无交点，则从constraintvec中去除
	cout << "\nSimplify";
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
							if (IsHaveIntersection(i, j, k, *iter)) {
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
}

void XLinearConstrainter::Print() {
	for (auto& i : constraintvec) {
		std::get<0>(i).Print();
		cout << " id1:" << std::get<1>(i) << " id2:" << std::get<2>(i);
	}
}