#pragma once
#include "soldier.h"

class CWizard :
	public CSoldier
{
private:
	bool			m_bDie;
	CObj*			m_pTower;

public:
	void	SetTower(CObj*	_pTower){ m_pTower = _pTower; }
	void	SetDie(bool _bDie){ m_bDie = _bDie; }
	void	Leveling();
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CWizard(void);
	~CWizard(void);
};
