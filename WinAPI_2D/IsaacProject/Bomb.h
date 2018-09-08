#pragma once
#include "bullet.h"

class CBomb :
	public CBullet
{
private:
	DWORD	m_dwBombTime;
	DWORD	m_dwExpTime;
	FRAME	m_tExpFrame;
	INFO	m_tColInfo;
	bool	m_bExplosion;
	bool	m_bBombitSelf;
	bool	m_bExpColBox;
public:
	bool GetExplosion(){ return m_bExplosion; }
	RECT GetCOlRect();
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CBomb(void);
	~CBomb(void);
};
