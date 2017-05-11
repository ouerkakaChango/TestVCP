#include "XLinearConstrainter.h"

void XLinearConstrainter::PushLine(float y1, float x1, float y2, float x2, int vid1, int vid2) {
	if (NearlyEqualf(y1, y2)) {
		constraintvec.push_back(XIDLine(XLine(y1),vid1,vid2));
	}
	else {
		float k = (x2 - x1) / (y2 - y1);
		float t = x1 - k*y1;
		constraintvec.push_back(XIDLine(XLine(k, t), vid1, vid2));
	}
}

void XLinearConstrainter::Simplify() {
	//???
	//将不需要的直线除去
	//1.取i,j=i+1,k=j+1的三点组成的两线(i,j)(j,k),获取射线对。若(i,j)或(j,k)已不在constraintvec中，则continue;
	//2.遍历constraintvec中其他非(i,j)(j,k)的线，看与射线对若无交点，则从constraintvec中去除
	cout << "\nSimplify";
}

void XLinearConstrainter::Print() {
	for (auto& i : constraintvec) {
		std::get<0>(i).Print();
		cout << " id1:" << std::get<1>(i) << " id2:" << std::get<2>(i);
	}
}