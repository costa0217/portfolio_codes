#pragma once
#include "lastwnd.h"

class CWin :
	public CLastWnd
{
private:
	float		m_fNamTime;
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CWin(void);
	~CWin(void);
};
