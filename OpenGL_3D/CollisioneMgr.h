#pragma once

#include "IncludeHeader.h"

class CCollide;
class CGameObject;
class CCollisioneMgr
{
	DECLARE_SINGLETON(CCollisioneMgr);
public:
	vector<CGameObject*>*	pvecObjects;

public:
	bool	CheckCollision(MATRIX mMy, MyVector vDeltaMove, CGameObject* pMy);
	bool	CheckCollisToCamera(MATRIX mMy, MyVector vDeltaMove, CCollide* myColl);


	bool	CheckBulletRayToAtkObj(MyVector* pvOut, MyVector* pvOutDir, int* nType, MyVector vRayStartPos, MyVector vRayDir);
	bool	CheckOBB(CCollide* myColl, MATRIX* myWorld, MyVector vDeltaMove);


public:
	CCollisioneMgr();
	~CCollisioneMgr();
};

