#pragma once

#include "Include.h"

class CObj;
class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	list<CObj*>*		m_pObjList;
	vector<TILE*>*		m_pVecTile;

public:
	void SetObjList(list<CObj*>* _pObjList){ m_pObjList = _pObjList; }
	void SetVecTile(vector<TILE*>* _pVecTile){ m_pVecTile = _pVecTile; }

public:
	bool MouseToTowerCollision();
	void TowerWndClear();

	void SoldierToMonster();
	void ArchorTowerToMonster();
	void ArrowToMonster();

	void MouseToUI();
	void MouseToWnd();

	void BossTowerFreezing();
private:
	CCollisionMgr(void);
	~CCollisionMgr(void);
};
