#pragma once
#include "monster.h"

class CHopper :
	public CMonster
{
private:
	list<CObj*>*	m_pBloodlist;

	DWORD	m_dwHopperTime;
	int		m_iPatternCircle;
	int			m_iDirLR;
	int			m_iDirUD;
public:
	void	SetBloodlist(list<CObj*>* _pBloodlist);
public:
	virtual void Initialize(void); 
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CHopper(void);
	~CHopper(void);
};
