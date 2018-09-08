#pragma once
#include "monster.h"

class CSnowman :
	public CMonster
{
private:
	float	m_fFreezingTime;
	bool	m_bFreezing;
public:
	bool	GetFreezingSkill(){ return m_bFreezing; }
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CSnowman(void);
	~CSnowman(void);
};
