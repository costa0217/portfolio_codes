#pragma once
#include "soldier.h"

class CFootMan :
	public CSoldier
{
private:
	UNITSTATS		m_tStats;			//이름, 체력, 최소공, 최대공, 이동속도
	float			m_fMaxHP;			//최대체력

	bool			m_bDie;
	bool			m_bDeadImg;

	int				m_iIndex;			//자기가 몇번 병사인지
	D3DXVECTOR3		m_vRellyPoint;		//랠리찍힌곳
	D3DXVECTOR3		m_vMyRelly;			//자기위치

	bool			m_bBattleMode;

	/* 상태 bool변수 */
	bool			m_bStand;
	bool			m_bMove;
	bool			m_bAttack;

	bool			m_bDamage;			//대미지 줄때

	bool			m_bHeal;			// 4-1 스킬 힐
	float			m_fHealTime;

	float			m_fDirTime;			// 가만히 있을때 왼쪽오른쪽 바라보기
	float			m_fAtkSpeed;

	CObj*			m_pMonster;			// 싸울 몬스터를 가져온다
	CObj*			m_pTower;			// 자기의 성을 가져옴

	bool			m_bSkill[2];		// 스킬 2가지
	int				m_iPlusDamage;
	bool			m_iHPup;

	float			m_fLifeTime;

	list<CObj*>*	m_pEffectList;

	bool			m_bEat;

	float			m_fNoAttackTime;

public:
	void		SetEat(bool _bEat){ m_bEat = _bEat; }
public:
	bool		GetSkillOnOff(int _iNum){ return m_bSkill[_iNum]; }
	void		SetSkillOnOff(int _iNum, bool _bSkill){ m_bSkill[_iNum] = _bSkill; }


public:			// 배틀
	void	SetBattleMonster(CObj* _pMonster){ m_pMonster = _pMonster; }
	CObj*	GetBattleMonster(){return m_pMonster;}

	void	SetFootManHP(int _iHP){ m_tStats.iHP = _iHP; }
	int		GetFootManHP(){ return m_tStats.iHP; }

public:
	void	SetMyTower(CObj* _pTower){ m_pTower = _pTower; }

	wstring	GetStateKey(){ return m_wstrStateKey; }
	bool	GetBattleMode(){ return m_bBattleMode; }
	void	SetBattleMode(bool _bBattleMode){ m_bBattleMode = _bBattleMode; }

	void	SetRellyPoint(D3DXVECTOR3 _vRelly){ m_vRellyPoint = _vRelly; }
	void	SetIndex(int _iIndex) { m_iIndex = _iIndex; }
	int		GetIndex(){ return m_iIndex; }

	//void	SetLevel(int _iLevel){ m_iLevel = _iLevel; }

	void	Leveling();


	bool	GetDie(){ return m_bDie; }
	void	SetDie(bool _bDie){ m_bDie = _bDie; }
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;

public:
	CFootMan(void);
	~CFootMan(void);
};
