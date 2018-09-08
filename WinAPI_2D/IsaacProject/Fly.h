#pragma once
#include "monster.h"

class CFly :
	public CMonster
{
private:
	DWORD	m_dwFlyTime;
	float	m_fAngle;
	
	CObj*	m_pPlayer;
	float	thetaAngle;

	float	PX;
	float	PY;
public:
	void	SetPlayer(CObj*	_pPlayer);
	//RECT	GetColRect();
public:
	virtual void Initialize(void); 
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CFly(void);
	~CFly(void);
};
