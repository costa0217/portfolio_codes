#pragma once
#include "LastWnd.h"

class CStopWnd :
	public CLastWnd
{
private:
	CObj*	m_pStopUI;
	bool*	m_bMeteoLV;
	bool*	m_bMillLV;

	int		m_iMeteoRGB[4];
	int		m_bMillRGB[4];
public:
	void	SetStopUI(CObj* _pStopUI) { m_pStopUI = _pStopUI; }
	void	KeyInput();

	void	SetMeteoLV(bool* _bMeteoLV){ m_bMeteoLV = _bMeteoLV; }
	void	SetMillLV(bool* _bMillLV){ m_bMillLV = _bMillLV; }

public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CStopWnd(void);
	~CStopWnd(void);
};
