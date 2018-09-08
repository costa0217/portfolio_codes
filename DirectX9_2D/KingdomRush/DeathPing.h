#pragma once
#include "projectile.h"

class CDeathPing :
	public CProjectile
{
private:
	CObj*			m_pMonster;
	float			m_fRazorLength;

	list<CObj*>*	m_pEffectList;

	bool			m_bDeath;
public:
	void			SetMonTarget(CObj* _pMonster){ m_pMonster = _pMonster; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CDeathPing(void);
	~CDeathPing(void);
};
