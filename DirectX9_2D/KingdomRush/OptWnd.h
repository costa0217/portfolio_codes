#pragma once
#include "lastwnd.h"

class COptWnd :
	public CLastWnd
{
private:
	CObj*	m_pOptUI;
	int		m_iImgNum;
	bool	m_bGoField;
public:
	void	SetOptUI(CObj* _pOptUI) { m_pOptUI = _pOptUI; }
	void	KeyInput();
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	COptWnd(void);
	~COptWnd(void);
};
