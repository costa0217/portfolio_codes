#pragma once
#include "monster.h"

class CShif :
	public CMonster
{
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CShif(void);
	~CShif(void);
};
