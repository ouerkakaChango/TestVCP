#include "XLinearConstrainter.h"
#include "XError.h"
void XLinearConstrainter::PushLine(float y1, float x1, float y2, float x2, int vid1, int vid2) {
	float a = x2 - x1, b = y1 - y2, c = y2*x1 - y1*x2;
	/*
	bool tgt = true;
	if (NearlyEqualf(b, 0.0f)) {
		//Ĭ��bΪ0ʱa������Ϊ0,a>0
		b = 0.0f;
		tgt = x1 < x2;
		if (a < 0) { tgt = !tgt; }//a<0
	}
	else {
		//ֻҪб�ʴ���,b>0
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
       //???
	//�ж������Ƿ��ཻ�����ཻ�Ͱѽ������outpoint

	//***
	return false;
}

bool XLinearConstrainter::IsNeedReserve(int i, int j, int k, const XIDLine& vidline) {
	//???
	//�ж�(i,j)(j,k)�����߶���ֱ��vidline���޽���
	//Ĭ�ϲ���������Ϊһ���ߵ����

	//***
	return false;
}

void XLinearConstrainter::Simplify() {
	//������Ҫ��ֱ�߳�ȥ
	//1.ȡi,j=i+1,k=j+1��������ɵ�����(i,j)(j,k),��ȡ���߶ԡ���(i,j)��(j,k)�Ѳ���constraintvec�У���continue;
	//2.����constraintvec��������(i,j)(j,k)���ߣ��������߶����޽��㣬���constraintvec��ȥ��
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
							if (IsNeedReserve(i, j, k, *iter)) {
								//����
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