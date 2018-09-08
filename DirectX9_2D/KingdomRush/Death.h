#pragma once
#include "effect.h"

class CDeath :
	public CEffect
{
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CDeath(void);
	~CDeath(void);
};
