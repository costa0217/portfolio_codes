#pragma once
#include "bullet.h"

class CBossSkill :
	public CBullet
{
private:
	INFO	m_tTargetInfo;
	INFO	m_tLegInfo;


	FRAME	m_tExpFrame;

	DWORD	m_dwExpTime;

	bool	m_bExplosion;
	int		m_iDeleteLeg;
public:
	RECT	GetCOlRect();
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CBossSkill(void);
	~CBossSkill(void);
};
