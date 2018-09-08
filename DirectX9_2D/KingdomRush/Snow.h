#pragma once
#include "obj.h"

class CSnow :
	public CObj
{
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CSnow(void);
	~CSnow(void);
};
