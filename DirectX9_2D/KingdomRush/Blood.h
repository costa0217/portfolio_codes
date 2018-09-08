#pragma once
#include "effect.h"

class CBlood :
	public CEffect
{
private:
	float		m_fBloodScale;
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CBlood(void);
	~CBlood(void);
};
