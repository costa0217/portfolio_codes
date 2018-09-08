#pragma once
#include "effect.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

class CBrokeIce :
	public CEffect
{
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CBrokeIce(void);
	~CBrokeIce(void);
};
