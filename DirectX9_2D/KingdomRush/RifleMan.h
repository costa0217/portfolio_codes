#pragma once
#include "soldier.h"

class CRifleMan :
	public CSoldier
{
private:
	bool			m_bDie;
	int				m_iIndex;

	CObj*			m_pMonster;
	bool			m_bLockOn;

	bool			m_bStateUp;
	bool			m_bStateDown;
	bool			m_bFirstAtk;
	bool			m_bFire;
	bool			m_bFireInterval;

	list<CObj*>*	m_pBulletList;

	float			m_fFireTime;

	int				m_iDamage;
	float			m_fAttackSpd;

	bool			m_bSkill[2];		// 스킬 2가지
	bool			m_bPoison;			// 4-1 독
	bool			m_bCritical;		// 4-2 크리
	bool			m_bSlow;			// 4-2 슬로우

	CObj*			m_pTower;

public:
	void		SetTower(CObj* _pTower){ m_pTower = _pTower; }

public:	//스킬
	bool		GetSkillOnOff(int _iNum){ return m_bSkill[_iNum]; }
	void		SetSkillOnOff(int _iNum, bool _bSkill){ m_bSkill[_iNum] = _bSkill; }
public:
	void	SetLockOn(bool _bLockOn){ m_bLockOn = _bLockOn; }
	bool	GetLockOn(){ return m_bLockOn; }

	void	SetLockMonster(CObj* _pMonster){ m_pMonster = _pMonster; }

	void	SetDamage(int _iDamage){ m_iDamage = _iDamage; }
	void	SetAttackSpd(float _fAttackSpd){ m_fAttackSpd = _fAttackSpd; }
public:
	void	SetDie(bool _bDie){ m_bDie = _bDie; }
	void	SetIndex(int _iIndex) { m_iIndex = _iIndex; }

	void	Leveling();
	void	SetDirImage(D3DXVECTOR3 _vMonster);
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CRifleMan(void);
	~CRifleMan(void);
};
