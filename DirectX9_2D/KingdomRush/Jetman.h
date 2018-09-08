#pragma once
#include "monster.h"

class CJetman :
	public CMonster
{
private:
	float	m_fBoosterTime;
	//bool	m_bBooster;
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CJetman(void);
	~CJetman(void);
};
