#pragma once
#include "obj.h"

class CTower :
	public CObj
{
protected:
	bool			m_bTowerClick;
	TOWERID			m_eTowerID;
	TWRSTATS		m_tTowerStat;
	FRAME			m_tTowerFrame;

	int				m_iLevel;
	bool			m_bLvl[LVL_END];
	bool			m_bLevelup;
	bool			m_bDestroyTower;

	bool			m_bSkill[2];

	bool			m_bFreezing;
	bool			m_bFreezImg;
	int				m_iFreezAtkCnt;

	list<CObj*>*	m_pEffectList;

public:
	bool		GetFreezing(){ return m_bFreezing; }
	void		SetFreezing(bool _bFreezing){ m_bFreezing = _bFreezing; }

	int			GetFreezAtkCnt(){ return m_iFreezAtkCnt; }
	void		SetFreezAtkCnt(int _iFreezAtkCnt){ m_iFreezAtkCnt = _iFreezAtkCnt; }

public:
	bool		GetSkillOnOff(int _iNum){ return m_bSkill[_iNum]; }
	void		SetSkillOnOff(int _iNum, bool _bSkill){ m_bSkill[_iNum] = _bSkill; }
public:
	TWRSTATS	GetTowerStats(){ return m_tTowerStat; }

	TOWERID		GetTowerID(){ return m_eTowerID; }

	bool		GetTowerClick(){ return m_bTowerClick; }
	void		SetTowerClick(bool _bClick){ m_bTowerClick = _bClick; }

	int			GetLevel(){ return m_iLevel; }
	void		SetLevelup(int _iLevel);

	void		SetDestroyTower(bool _Destroy) { m_bDestroyTower = _Destroy; }
public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Progress(void)		PURE;
	virtual void Render(void)		PURE;
	virtual void Release(void)		PURE;
public:
	CTower(void);
	virtual ~CTower(void);
};
