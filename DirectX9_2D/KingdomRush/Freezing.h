#pragma once
#include "effect.h"

class CFreezing :
	public CEffect
{
private:
	CObj*			m_pTower;
	list<CObj*>*	m_pEffectList;
public:
	void	SetTower(CObj* _pTower){ m_pTower = _pTower; }
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CFreezing(void);
	~CFreezing(void);
};
