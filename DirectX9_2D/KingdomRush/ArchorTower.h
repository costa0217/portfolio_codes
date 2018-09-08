#pragma once
#include "tower.h"

class CArchorTower :
	public CTower
{
private:
	bool			m_bArchorMake;

	bool			m_bWark;
	SHEETFRM		m_tWarkFrame;
	DWORD			m_dwWarkTime;

	list<CObj*>*	m_pSoldierList;
	vector<CObj*>	m_VecMySoldier;

	bool			m_bArchorLockOn;
	CObj*			m_pMonster;

	bool			m_bSkillInten;
	float			m_fIntengleTime;

public:		// ¿Œ≈ ±€ Ω∫≈≥
	bool		GetIntengle(){ return m_bSkillInten; }
	void		SetIntengle(bool _bSkillInten){ m_bSkillInten = _bSkillInten; }

	float		GetIntengleTime(){ return m_fIntengleTime; }
	void		SetIntengleTime(float _fIntengleTime){ m_fIntengleTime = _fIntengleTime; }

public:
	void		SetSoldierList( list<CObj*>* _pSoldierList) { m_pSoldierList = _pSoldierList; }

	bool		GetArchorLockOn(){return m_bArchorLockOn;}
	void		SetArchorLockOn(bool _bArchorLockOn){ m_bArchorLockOn = _bArchorLockOn; }

	void		SetLockOnMonster(CObj* _pMonster){ m_pMonster = _pMonster; }
	CObj*		GetLockOnMonster(){ return m_pMonster; }

private:
	void		Leveling();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CArchorTower(void);
	~CArchorTower(void);
};
