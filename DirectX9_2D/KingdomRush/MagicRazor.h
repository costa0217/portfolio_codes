#pragma once
#include "projectile.h"

class CMagicRazor :
	public CProjectile
{
private:
	CObj*			m_pMonster;
	float			m_fRazorLength;

	list<CObj*>*	m_pEffectList;

	int				m_iDamage;

	bool			m_bZizizic;

public:
	void			SetMonTarget(CObj* _pMonster){ m_pMonster = _pMonster; }

	void			SetDamage(int _iDamage){ m_iDamage = _iDamage; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CMagicRazor(void);
	~CMagicRazor(void);
};
