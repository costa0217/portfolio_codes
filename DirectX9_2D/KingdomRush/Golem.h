#pragma once
#include "soldier.h"

class CGolem :
	public CSoldier
{
private:
	UNITSTATS		m_tStats;			//이름, 체력, 최소공, 최대공, 이동속도
	float			m_fMaxHP;			//최대체력

	D3DXVECTOR3		m_vRellyPoint;		//랠리찍힌곳

	bool			m_bDie;
	bool			m_bDeadImg;

	bool			m_bBattleMode;

	/* 상태 bool변수 */
	bool			m_bBirth;
	bool			m_bStand;
	bool			m_bMove;
	bool			m_bAttack;

	bool			m_bDamage;			//대미지 줄때

	CObj*			m_pMonster;			// 싸울 몬스터를 가져온다
	list<CObj*>*	m_pEffectList;

public:			// 배틀
	wstring	GetStateKey(){ return m_wstrStateKey; }

	void	SetBattleMonster(CObj* _pMonster){ m_pMonster = _pMonster; }
	CObj*	GetBattleMonster(){return m_pMonster;}

	bool	GetBattleMode(){ return m_bBattleMode; }
	void	SetBattleMode(bool _bBattleMode){ m_bBattleMode = _bBattleMode; }

	void	SetGolemHP(int _iHP){ m_tStats.iHP = _iHP; }
	int		GetGolemHP(){ return m_tStats.iHP; }

public:
	void	SetRellyPoint(D3DXVECTOR3 _vRelly){ m_vRellyPoint = _vRelly; }

	bool	GetDie(){ return m_bDie; }
	void	SetDie(bool _bDie){ m_bDie = _bDie; }
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CGolem(void);
	~CGolem(void);

};
