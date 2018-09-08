#pragma once
#include "effect.h"

class CClick :
	public CEffect
{
private:
	CObj*	m_pTower;
public:
	void	SetTower(CObj* _pTower){ m_pTower = _pTower; }
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CClick(void);
	~CClick(void);
};
