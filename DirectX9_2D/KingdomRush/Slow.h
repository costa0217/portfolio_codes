#pragma once
#include "effect.h"

class CSlow :
	public CEffect
{
private:
	CObj*	m_pMonster;
public:
	void	SetMonster(CObj* _pMonster){ m_pMonster = _pMonster; }
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CSlow(void);
	~CSlow(void);
};
