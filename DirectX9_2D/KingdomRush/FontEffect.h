#pragma once
#include "effect.h"

class CFontEffect :
	public CEffect
{
private:
	float			m_fTime;
	D3DXMATRIX		m_matFontTrans;
	wstring			m_wstrFont;
	wstring		m_wstrEffect;
public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void)	;
	virtual void	Render(void)	;
	virtual void	Release(void)	;
public:
	CFontEffect(void);
	~CFontEffect(void);
};
