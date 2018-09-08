#pragma once
#include "monster.h"

class CObj;
class CBoss :
	public CMonster
{
private:

	INFO			m_tHealthbar;
	list<CObj*>*	m_pBulletlist;
	CObj*			m_pPlayer;

	DWORD	m_dwBossTime;
	
	DWORD	m_dwPattBasicTime;
	DWORD	m_dwPattFstTime;
	DWORD	m_dwPattSecTime;
	DWORD	m_dwPattSec2Time;
	DWORD	m_dwPattTrdTime;
	DWORD	m_dwPattFndTime;

	int		m_iPattFndTime;

	float	m_iSecPatt;

	int		m_iDirLR;
	int		m_iDirUD;

	float	PX;
	float	PY;

	int		m_iPatternCnt; /* 패턴을 하기위한 조건 */
	int		m_iPatternNum; /* 몇번째 패턴 */

	bool	m_bNotAttack;

	int		m_iMaxHP;
private:
	void	Basicpattern();
	void	Movepattern();
public:
	void	SetBulletlist(list<CObj*>* _pBulletlist);
	void	SetPlayer(CObj*	_pPlayer);

	bool	GetNotAttack() { return m_bNotAttack; }
public:
	virtual void Initialize(void);  
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CBoss(void);
	~CBoss(void);
};

