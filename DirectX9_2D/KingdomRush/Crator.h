#pragma once
#include "projectile.h"

class CCrator :
	public CProjectile
{
private:
	int		m_iCnt;
	float	m_fRange;
	int		m_iDamage;
	bool	m_bOneCircle;

public:
	bool	GetOneCircle(){ return m_bOneCircle; }
	float	GetRange(){ return m_fRange; }
	int		GetDamage(){ return m_iDamage; }

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CCrator(void);
	~CCrator(void);
};
