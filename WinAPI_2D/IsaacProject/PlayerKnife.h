#pragma once
#include "bullet.h"

class CPlayerKnife :
	public CBullet
{
private:
	CObj*	m_pPlayer;
	INFO	m_tPlayInfo;

	int		m_iDirX;
	int		m_iDirY;
	bool	m_bSting;

	float	m_fTime;

	int		m_iDirKnife;
	//DWORD	m_dwFamFlyTime;
public:
	void SetPlayer(CObj* _pPlayer){ m_pPlayer = _pPlayer; }

	RECT GetCOlRect();
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CPlayerKnife(void);
	~CPlayerKnife(void);
};
