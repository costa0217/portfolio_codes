#pragma once
#include "monster.h"

class CMiniBoss :
	public CMonster
{
private:
	list<CObj*>*	m_pMonsterlist;
	list<CObj*>*	m_pBloodlist;
	DWORD			m_dwMiniBossTime;
	MINIBOSSDIR		m_eMiniDIR;
public:
	void	SetMonsterlist(list<CObj*>* _pMonsterlist);

	void	SetMiniDIR(MINIBOSSDIR	_eMiniDIR) { m_eMiniDIR = _eMiniDIR; }
	MINIBOSSDIR	GetMiniDIR(){ return m_eMiniDIR; }

	void	SetBloodlist(list<CObj*>* _pBloodlist);
public:
	virtual void Initialize(void);  
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CMiniBoss(void);
	~CMiniBoss(void);
};
