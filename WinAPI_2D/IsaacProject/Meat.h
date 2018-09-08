#pragma once
#include "monster.h"

class CMeat :
	public CMonster
{
private:
	DWORD	m_dwMeatTime;
	DWORD	m_dwDIRTime;
	list<CObj*>*	m_pBulletlist;
	list<CObj*>*	m_pBloodlist;

	int				m_iDirLR;
	int				m_iDirUD;

	int				m_iPatternCircle;
public:
	void	SetBulletlist(list<CObj*>* _pBulletlist);
	void	SetBloodlist(list<CObj*>* _pBloodlist);
public:
	virtual void Initialize(void); 
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CMeat(void);
	~CMeat(void);
};
