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
	void CObjMgr::PlayerTileCollision();			/*�÷��̾� - Ÿ��*/
	void CObjMgr::PlayerMonsterCollision();			/*�÷��̾� - ����*/
	void CObjMgr::PlayerBulletCollision();			/*�÷��̾� - �Ѿ�*/

	void CObjMgr::BulletTileCollision();			/*�Ѿ� - Ÿ��*/
	void CObjMgr::BulletBombCollision();			/*�Ѿ� - ��ź*/
	void CObjMgr::BulletMonsterCollision();			/*�Ѿ� - ����*/

	void CObjMgr::TileMonsterCollision();			/*Ÿ�� - ����*/

	void CObjMgr::PlayerItemCollision();			/*�÷��̾� - ������*/
	
	

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
