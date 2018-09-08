#pragma once
#include "bullet.h"

class CPlayerSnake :
	public CBullet
{
private:
	INFO	m_tShadowInfo;
	float m_fMyAngle;

	bool	m_bPoof;
	DWORD	m_dwBulletTime;
public:
	RECT GetCOlRect();
	void SetPoof(bool _bPoof){m_bPoof = _bPoof;}
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);

public:
	CPlayerSnake(void);
	~CPlayerSnake(void);
};
