#pragma once
#include "bullet.h"

class CPlayerBig :
	public CBullet
{
private:
	CObj*	m_pPlayer;	
	INFO	m_tShadowInfo;
	float	m_fOriX;
	float	m_fOriY;
	float	m_fTime;


	bool	m_bPoof;

	DWORD	m_dwBulletTime;

public:
	float DistanceMake();
	RECT GetCOlRect();
	void SetPoof(bool _bPoof){
		m_bPoof = _bPoof;}
public:
	void SetPlayer(CObj* _pPlayer){ m_pPlayer = _pPlayer; }
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);

public:
	CPlayerBig(void);
	~CPlayerBig(void);
};
