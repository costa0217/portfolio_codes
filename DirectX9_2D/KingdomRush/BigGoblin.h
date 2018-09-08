#pragma once
#include "monster.h"

class CBigGoblin :
	public CMonster
{
private:
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CBigGoblin(void);
	~CBigGoblin(void);
};
