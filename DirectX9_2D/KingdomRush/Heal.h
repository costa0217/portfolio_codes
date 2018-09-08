#pragma once
#include "effect.h"

class CHeal :
	public CEffect
{
private:
	CObj*		m_pFootMan;
public:
	void		SetFootMan(CObj* _pFootMan){ m_pFootMan = _pFootMan; }
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CHeal(void);
	~CHeal(void);
};
