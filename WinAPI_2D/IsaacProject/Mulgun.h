#pragma once
#include "monster.h"

class CMulgun :
	public CMonster
{
private:
	list<CObj*>*	m_pMonsterlist;
	list<CObj*>*	m_pBloodlist;

	CObj*			m_pPlayer;
	DWORD	m_dwMulgunTime;
	int		m_iPatternCircle;

	float	PX;
	float	PY;
	float	thetaAngle;
	int		m_iCircle;
public:
	void	SetMonsterlist(list<CObj*>* _pMonsterlist);
	void	SetBloodlist(list<CObj*>* _pBloodlist);
	void	SetPlayer(CObj*	_pPlayer);
public:
	virtual void Initialize(void);  
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CMulgun(void);
	~CMulgun(void);
};
