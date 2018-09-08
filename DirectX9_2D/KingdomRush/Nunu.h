#pragma once
#include "monster.h"

class CNunu :
	public CMonster
{
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CNunu(void);
	~CNunu(void);
};
