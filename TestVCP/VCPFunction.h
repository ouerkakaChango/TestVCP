#ifndef VCPFUNCTION_H
#define VCPFUNCTION_H
#include "KVec3.h"
class VCPFunction
{
public:

};
class FunctionManager {
public:
	static FunctionManager& GetInstance();
	KVec3 Func(KVec3 s,KVec3 e, int funcid, int intervallength, int temid);
};
#define FUNCMGR FunctionManager::GetInstance()
#endif

