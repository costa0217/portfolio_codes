#pragma once
#include "ui.h"

class COptionButt :
	public CUI
{
private:
	float			m_fButtResetTime;
	list<CObj*>*	m_pLastWndList;
public:
	void	MouseMove();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	COptionButt(void);
	~COptionButt(void);
};
