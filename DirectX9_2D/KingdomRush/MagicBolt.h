#pragma once
#include "projectile.h"

class CMagicBolt :
	public CProjectile
{
private:
	list<CObj*>*		m_pEffectList;
	CObj*				m_pMonTarget;
	float				m_fTime;

	int					m_iDamage;
	bool				m_bHit;

	float				m_fBoltScale;
public:
	void		SetMonTarget(CObj* _pMonTarget){ m_pMonTarget = _pMonTarget; }
	CObj*		GetMonTarget(){ return m_pMonTarget; }

	void		SetHit(bool _bHit){ m_bHit = _bHit; }

	void		SetDamage(int _iDamage){ m_iDamage = _iDamage; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CMagicBolt(void);
	~CMagicBolt(void);
};
