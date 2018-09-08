#pragma once
#include "projectile.h"

class CArrow :
	public CProjectile
{
private:
	CObj*			m_pMonTarget;
	list<CObj*>*	m_pEffectList;
	float			m_fTime;


	bool			m_bHit;
	bool			m_bGuided;		//유도일때
	bool			m_bFormula;		//포물선일때

	bool			m_bPoisonArr;

	int				m_iDamage;

	D3DXVECTOR3		m_vArchorPos;			// 궁병 위치

	D3DXVECTOR3		m_vMonInitPos;			// 몬스터 위치
	D3DXVECTOR3		m_vPreArrPos;			// 이전 화살위치

	float			m_fSpeedY;
public:
	void	SetPoisonArrow(bool _bPosionArr){ m_bPoisonArr = _bPosionArr; }
public:
	void	SetMonTarget(CObj* _pMonTarget){ m_pMonTarget = _pMonTarget; }
	CObj*	GetMonTarget(){ return m_pMonTarget; }

	void	SetHit(bool _bHit){ m_bHit = _bHit; }

	void	SetDamage(int _iDamage){ m_iDamage = _iDamage; }

public:
	float	ParabolaFormula(D3DXVECTOR3 _vMon, D3DXVECTOR3 _vAcp);
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;

public:
	CArrow(void);
	~CArrow(void);
};
