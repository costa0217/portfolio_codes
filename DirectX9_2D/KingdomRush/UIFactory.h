#pragma once

#include "Include.h"

class CUI;
class CObj;
template<typename T>
class CUIFactory
{
public:
	static CObj*	CreateWnd2(list<CObj*>* _pTowerList, CObj* _pTower)
	{
		CObj*	pObj	= new T;
		((CTowerWnd2*)pObj)->SetTowerList(_pTowerList);
		((CTowerWnd2*)pObj)->SetTower(_pTower);
		pObj->Initialize();

		return pObj;
	}
public:
	CUIFactory(void){ }
	~CUIFactory(void){}
};
