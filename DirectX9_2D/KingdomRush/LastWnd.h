#pragma once
#include "obj.h"

class CLastWnd :
	public CObj
{
protected:
	WNDID	m_eWndType;
	bool	m_bClick;

public:
	WNDID	GetWndType(){ return m_eWndType; }

	void	SetClick(bool _bClick){ m_bClick = _bClick; }
	bool	GetClick(){ return m_bClick; }

public:
	virtual HRESULT Initialize(void)	PURE;
	virtual int		Progress(void)		PURE;
	virtual void	Render(void)		PURE;
	virtual void	Release(void)		PURE;
public:
	CLastWnd(void);
	virtual ~CLastWnd(void);
};
