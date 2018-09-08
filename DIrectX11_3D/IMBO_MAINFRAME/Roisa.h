#pragma once
#include "GameObject.h"
class CRoisa :
	public CGameObject
{
public:
	CGameObject*	m_pTempPlayer{ nullptr };

	//void SetFirstAction(bool b) { m_bFirstAction = b; }
	//bool GetFirstAction() { return m_bFirstAction; }
private:
	//bool m_bFirstAction{ false };

	float		m_fSpeed{ 0.f };
	UINT		m_nAnimNum{ 0 };
	XMFLOAT3	m_f3Diraction;
	float		m_fMyAngle{ 0.f };

	float	    m_fAngleY{ 0.f };
	float		m_fAccSkillTime{ 0.f };
	float		m_fSk2Time{ 0.f };
	float		m_fSk2Speed{ 0.f };
	XMFLOAT3	m_xmf3Sk2Dir;

	bool		m_bAttack{ false };
	bool		m_bSkill{ false };
	WORD		m_nPatternNum{ 0 };
public:
	virtual void Animate(float fTimeElapsed);
	virtual void RegistToContainer();

	bool GetDemaged(int iDemege);
	void TransferCollisioinData(int target_slot_id, int skillnum);
public:
	virtual void PhisicsLogic(map<utag, list<CGameObject*>>& mlpObject, float fDeltaTime);
	virtual void GetSkilled(int nSkill);
private:
	void	UpdatePattern(float fTimeElapsed);

private:
	map<string, vector<CGameObject*>>	m_mapSkill;

private:
	void	ShootMeteo(float fAngle);
	void	ShootExplosion();
	void	ShootBlizzard();

public:
	CRoisa(string name, tag t = tag::TAG_DEFAULT, CGameObject* pTarget = nullptr);
	virtual ~CRoisa();
};

