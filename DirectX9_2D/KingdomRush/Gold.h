#pragma once
#include "effect.h"

class CGold :
	public CEffect
{
private:;
	float		m_fStayTime;
	float		m_fAlphaTime;
	int			m_iGold;
	int			m_iAlpha;

public:
	void		SetGold(int _iGold){ m_iGold = _iGold; }

public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CGold(void);
	~CGold(void);
};
