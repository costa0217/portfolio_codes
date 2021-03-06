#pragma once
#include "monster.h"

class CBabySpider :
	public CMonster
{
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CBabySpider(void);
	~CBabySpider(void);
};
