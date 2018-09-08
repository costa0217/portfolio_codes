#pragma once

#include "Include.h"

class CObj;
class CScene;
class CSceneMgr
{
private:
	static CSceneMgr* m_pInst;
	CScene*			  m_pScene[SC_END];

	CObj*			m_pPlayer;
	CObj*			m_pUI;
	vector<CObj*>	m_VacHart;

	SCENEID			m_eCurrentScene;

public:
	void SetScene(SCENEID  _eID);
	//list<CObj*>* GetObjList(){ return m_ObjList; }

	CObj*	GetPlayer() { return m_pPlayer; }
	CObj*	GetUI()	{ return m_pUI; }
	vector<CObj*>*	GetVacHart() { return &m_VacHart; }
		
public:

	void Progress(void);
	void Render(HDC hdc);
	void Release(void);

public:
	static CSceneMgr* GetInst(void)
	{
		if(m_pInst == NULL)
			m_pInst = new CSceneMgr;

		return m_pInst;
	}

	void DestroyInst(void)
	{
		delete m_pInst;
		m_pInst = NULL;
	}

private:
	CSceneMgr(void);
	~CSceneMgr(void);
};
