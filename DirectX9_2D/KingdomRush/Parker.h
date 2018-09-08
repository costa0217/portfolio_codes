#pragma once
#include "monster.h"

class CParker :
	public CMonster
{
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CParker(void);
	~CParker(void);
};
