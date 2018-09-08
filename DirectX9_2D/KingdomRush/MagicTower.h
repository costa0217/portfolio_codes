#pragma once
#include "tower.h"

class CMagicTower :
	public CTower
{
private:
	bool		m_bMagicMake;

	bool		m_bWark;
	SHEETFRM	m_tWarkFrame;
	DWORD		m_dwWarkTime;

	D3DXVECTOR3		m_vRellyPoint;

	list<CObj*>*	m_pBulletList;
	list<CObj*>*	m_pSoldierList;
	list<CObj*>*	m_pEffectList;

	vector<CObj*>	m_VecMySoldier;

	bool			m_bMagicLockOn;
	CObj*			m_pMonster;

	bool			m_bFire;
	bool			m_bFireInterval;
	float			m_fFireTime;

	bool			m_bDeathPing;	// 4-2 스킬 데스핑
	float			m_fDeathTime;

	bool			m_bTeleport;	// 4-2 스킬 텔포
	float			m_fTelpoTime;

	bool			m_bGolemSummon;	// 4-1 스킬 골렘
	bool			m_bGolemDeath;
	float			m_fGolemRespawn;

	vector<TILE*>*	m_pVecTile;

public:
	int		MagicNumbering();

	bool	GetMagicLockOn(){ return m_bMagicLockOn; }
	void	SetMagicLockOn(bool _bMagicLockOn){ m_bMagicLockOn = _bMagicLockOn; }

	CObj*	GetLockOnMonster(){ return m_pMonster; }
	void	SetLockOnMonster(CObj*	_pMonster){ m_pMonster = _pMonster; }
public:
	void		SetSoldierList( list<CObj*>* _pSoldierList) { m_pSoldierList = _pSoldierList; }
public:
	void		Leveling();
	void		SetRellyPoint(D3DXVECTOR3 _vRelly){ m_vRellyPoint = _vRelly; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CMagicTower(void);
	~CMagicTower(void);
};
