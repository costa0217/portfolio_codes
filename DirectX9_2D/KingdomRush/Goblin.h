#pragma once
#include "monster.h"

class CGoblin :
	public CMonster
{
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CGoblin(void);
	~CGoblin(void);
};
