#pragma once
#include "GameObject.h"
class CSCOriTown;
class CLesserGiant :
	public CGameObject
{

public:
	CGameObject*	m_pTempPlayer{ nullptr };

	void SetAnimNum(UINT n) { m_nAnimNum = n; }
	UINT GetAnimNum() { return m_nAnimNum; }
	void SetFirstAction(bool b) { m_bFirstAction = b; }
	bool GetFirstAction() { return m_bFirstAction; }
	//void SetCurHP(int hp) { m_iCurHP = hp; }
	void SetTarget(CGameObject* pTarget) { m_pTempPlayer = pTarget;	};
private:

	bool m_bFirstAction{ false };
	float		m_fSpeed{ 0.f };
	UINT		m_nAnimNum{ 0 };
	XMFLOAT3	m_f3Diraction;

	float	    m_fAngleY{ 0.f };
	float		m_fAccSkillTime{ 0.f };
	float		m_fSk2Time{ 0.f };
	float		m_fSk2Speed{ 0.f };
	XMFLOAT3	m_xmf3Sk2Dir;

	bool		m_bAttack{ false };
	bool		m_bSkill{ false };
	WORD		m_nPatternNum{ 1 };

public:
	virtual void Animate(float fTimeElapsed);
	virtual void RegistToContainer();

public:
	void TransferCollisioinData(int target_slot_id, int skillnum);
	virtual void PhisicsLogic(map<utag, list<CGameObject*>>& mlpObject, float fDeltaTime);

	bool GetDemaged(int iDemege);
	virtual void GetSkilled(int nSkill);
	virtual int GetSlotID() { return 5; }
private:
	void	UpdatePattern(float fTimeElapsed);

public:
	CLesserGiant(string name, tag t = tag::TAG_DEFAULT, CGameObject* pTarget = nullptr);
	~CLesserGiant();
};

