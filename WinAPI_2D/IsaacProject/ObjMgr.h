#pragma once
#include "Include.h"

class CObj;
class CObjMgr
{
private:
	list<CObj*>*		m_pObjList;
	vector<TILE*>*		m_pTileVec;

	bool				m_bNextRoom;
	
public:
	void SetObjList(list<CObj*>*  pObjList); 
	void SetTileVector(vector<TILE*>*  pVecTile);

	list<CObj*>* GetObjList() { return m_pObjList; }	

	void SetNextRoom(bool _bNextRoom){ m_bNextRoom = _bNextRoom; }
	bool GetNextRoom(){ return m_bNextRoom; }

public:
	void CObjMgr::PlayerTileCollision();			/*플레이어 - 타일*/
	void CObjMgr::PlayerMonsterCollision();			/*플레이어 - 몬스터*/
	void CObjMgr::PlayerBulletCollision();			/*플레이어 - 총알*/

	void CObjMgr::BulletTileCollision();			/*총알 - 타일*/
	void CObjMgr::BulletBombCollision();			/*총알 - 폭탄*/
	void CObjMgr::BulletMonsterCollision();			/*총알 - 몬스터*/

	void CObjMgr::TileMonsterCollision();			/*타일 - 몬스터*/

	void CObjMgr::PlayerItemCollision();			/*플레이어 - 아이템*/
	
	

private:
	static	CObjMgr*	m_pInst;

public:
	static	CObjMgr*	GetInst(void)
	{
		if(m_pInst == NULL)
			m_pInst = new CObjMgr;
		return m_pInst;
	}
	void DestroyInst(void)
	{
		delete m_pInst;
		m_pInst = NULL;
	}

private:
	CObjMgr(void);
	~CObjMgr(void);
};
