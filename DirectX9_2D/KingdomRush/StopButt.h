#pragma once
#include "ui.h"

class CStopButt :
	public CUI
{
private:
	CObj*			m_pMeteoUI;
	CObj*			m_pMillUI;
	

	float			m_fButtResetTime;

	list<CObj*>*	m_pLastWndList;

	bool	m_bMeteoLV[4];
	bool	m_bMillLV[4];

	bool	m_bLevelPass;

public:
	void	SetMeteoUI(CObj* _pMeteoUI){ m_pMeteoUI = _pMeteoUI; }
	void	SetMillUI(CObj* _pMillUI){ m_pMillUI = _pMillUI; }
public:
	void	MouseMove();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CStopButt(void);
	~CStopButt(void);
};
