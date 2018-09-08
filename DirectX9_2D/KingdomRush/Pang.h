#pragma once
#include "monster.h"

class CPang :
	public CMonster
{
private:
	float		m_fFrameSpd;
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CPang(void);
	~CPang(void);
};
