#pragma once
#include "effect.h"

class CSmoke :
	public CEffect
{
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CSmoke(void);
	~CSmoke(void);
};
