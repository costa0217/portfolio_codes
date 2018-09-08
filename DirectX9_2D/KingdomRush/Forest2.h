#pragma once
#include "obj.h"

class CForest2 :
	public CObj
{
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CForest2(void);
	~CForest2(void);
};
