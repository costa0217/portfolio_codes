#pragma once
#include "tower.h"

class CBombTower :
	public CTower
{
private:
	bool			m_bWark;
	SHEETFRM		m_tWarkFrame;
	DWORD			m_dwWarkTime;

	list<CObj*>*	m_pBulletList;
	list<CObj*>*	m_pEffectList;
	list<CObj*>*	m_pSkillList;

	bool			m_bBombLockOn;
	CObj*			m_pMonster;

	bool			m_bFire;
	bool			m_bFireInterval;
	float			m_fFireTime;

	bool			m_bGuidedMisile;
	float			m_fGMisileTime;
	int				m_iGMisileCnt;

	bool			m_bChainLight;

public:
	bool		GetBombLockOn(){ return m_bBombLockOn; }
	void		SetBombLockOn(bool _bBombLockOn){ m_bBombLockOn = _bBombLockOn; }

	CObj*		GetLockOnMonster(){ return m_pMonster; }
	void		SetLockOnMonster(CObj*	_pMonster){ m_pMonster = _pMonster; }
public:
	void		Leveling();
	int			BoomNumbering();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CBombTower(void);
	~CBombTower(void);
};
