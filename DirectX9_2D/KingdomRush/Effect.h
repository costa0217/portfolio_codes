#pragma once
#include "obj.h"

class CEffect :
	public CObj
{
protected:
	EFFECTID	m_eEffectID;

public:
	EFFECTID	GetEffectID(){ return m_eEffectID; }
public:
	virtual HRESULT Initialize(void)	PURE;
	virtual int		Progress(void)		PURE;
	virtual void	Render(void)		PURE;
	virtual void	Release(void)		PURE;
public:
	CEffect(void);
	virtual ~CEffect(void);
};
