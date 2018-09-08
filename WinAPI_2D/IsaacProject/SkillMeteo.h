#pragma once
#include "bullet.h"

class CSkillMeteo :
	public CBullet
{
private:
	INFO	m_tTargetInfo;
	INFO	m_tMeteoInfo;


	FRAME	m_tExpFrame;

	DWORD	m_dwExpTime;
	DWORD	m_dwMeteoTime;

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
	CSkillMeteo(void);
	~CSkillMeteo(void);
};
