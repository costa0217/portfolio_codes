#pragma once
#include "block.h"

class CFire :
	public CBlock
{
private:
	DWORD	m_dwFireTime;
	DWORD	m_dwFirePoofTime;

	FRAME	m_tPoofFrame;

	bool	m_bFireFade;
	bool	m_bFirePoof;
public:
	void 	SetFireFade(bool _bFireFade){ m_bFireFade = _bFireFade;}
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CFire(void);
	~CFire(void);
};
