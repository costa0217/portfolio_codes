#pragma once
#include "obj.h"

class CForest1 :
	public CObj
{
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CForest1(void);
	~CForest1(void);
};
