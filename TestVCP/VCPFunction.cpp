#include "VCPFunction.h"

FunctionManager& FunctionManager::GetInstance() {
	static FunctionManager x;
	return x;
}

KVec3 FunctionManager::Func(KVec3 s, KVec3 e, int funcid, int intervallength, int temid) {
	if (funcid < 0) { return s; }
	else if (funcid == 0) {//linear
		KVec3 delta = (e - s) / intervallength;
		return s + delta*temid;
	}
}