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
	//������Ҫ��ֱ�߳�ȥ
	//1.ȡi,j=i+1,k=j+1��������ɵ�����(i,j)(j,k),��ȡ���߶ԡ���(i,j)��(j,k)�Ѳ���constraintvec�У���continue;
	//2.����constraintvec��������(i,j)(j,k)���ߣ��������߶����޽��㣬���constraintvec��ȥ��
	cout << "\nSimplify";
}

void XLinearConstrainter::Print() {
	for (auto& i : constraintvec) {
		std::get<0>(i).Print();
		cout << " id1:" << std::get<1>(i) << " id2:" << std::get<2>(i);
	}
}