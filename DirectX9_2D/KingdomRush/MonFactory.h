#pragma once

#include "Include.h"

class CMonster;
class CObj;
template<typename T>
class CMonFactory
{
public:
	static CObj*	CreateMonster(vector<TILE*>* _pVecTile, vector<int>* _pVecRoute)
	{
		CObj*	pObj	= new T;
		((CMonster*)pObj)->SetVecTile(_pVecTile);
		((CMonster*)pObj)->SetVecRoute(_pVecRoute);		
		pObj->Initialize();

		return pObj;
	}
public:
	CMonFactory(void){}
	~CMonFactory(void){}
};

