#pragma once
#include "soldier.h"

class CGolem :
	public CSoldier
{
private:
	UNITSTATS		m_tStats;			//�̸�, ü��, �ּҰ�, �ִ��, �̵��ӵ�
	float			m_fMaxHP;			//�ִ�ü��

	D3DXVECTOR3		m_vRellyPoint;		//����������

	bool			m_bDie;
	bool			m_bDeadImg;

	bool			m_bBattleMode;

	/* ���� bool���� */
	bool			m_bBirth;
	bool			m_bStand;
	bool			m_bMove;
	bool			m_bAttack;

	bool			m_bDamage;			//����� �ٶ�

	CObj*			m_pMonster;			// �ο� ���͸� �����´�
	list<CObj*>*	m_pEffectList;

public:			// ��Ʋ
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
