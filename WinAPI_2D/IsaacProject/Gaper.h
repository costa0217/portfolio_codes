#pragma once
#include "monster.h"

class CGaper :
	public CMonster
{
private:
	CObj*			m_pPlayer;
	list<CObj*>*	m_pBulletlist;
	list<CObj*>*	m_pBloodlist;

	DWORD	m_dwGaperTime;
	float	thetaAngle;
	float	m_fVibrateX;
	float	m_fVibrateY;

	bool	m_bLockOn;
	DWORD	m_dwShotTime;

public:
	void	SetPlayer(CObj*	_pPlayer);
	void	SetBulletlist(list<CObj*>* _pBulletlist);
	void	SetBloodlist(list<CObj*>* _pBloodlist);
public:
	virtual void Initialize(void); 
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CGaper(void);
	~CGaper(void);
};