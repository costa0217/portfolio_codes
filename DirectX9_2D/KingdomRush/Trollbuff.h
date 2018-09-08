#pragma once
#include "monster.h"

class CTrollbuff :
	public CMonster
{

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CTrollbuff(void);
	~CTrollbuff(void);
};
