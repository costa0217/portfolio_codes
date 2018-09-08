#pragma once
#include "GameObject.h"
class CDementor :
	public CGameObject
{

public:
	bool Begin();
	virtual void Animate(float fTimeElapsed);
	virtual bool End();

private:
	void	KeyInput(float fDeltaTime);
	void	PushServerData(float x, float y, float z, float fAngleY, int nAnimNum);
	void	GetServerData(float fTimeElapsed);
	void	SetupAnimation(DWORD dwDirection);
	void	Jumping(float fDeltaTime);
	void	SetWeapon();

	void	UpdateSkill();
	void	ShootArrow(bool bStrong, float fAngle = 0.f);
private:
	//skill 변수
	float m_fSkillTime{ 0.f };
	float m_fSkill1EndTime{ 0.f };//공격력 증가

	bool	m_bSelRangeMode{ false };
	XMFLOAT3 m_xmf3ClickPos;

private:
	float	m_fTranslateTime{ 0.f };
	int		m_iAnimNum{ 0 };
	bool	m_bSprit{ false };
	float	m_fJumpTime{ 0.f };

	float	m_fWalkEffectTime{ 0.f };
private:

	UINT	m_nAnimNum{ 0 };
	float	m_fAngleY{ 0.f };
	float	m_fAngleX{ 0.f };
	int		m_iPawnNum{ -1 };

	CGameObject*	m_pWeapon{ nullptr };

private:	//스킬
	map<string, vector<CGameObject*>>	m_mapSkill;

	//bool	m_bSkill[SKM_END];
	bool	m_bSkill{ false };

private:
	XMMATRIX	m_xmatrixRotate;
	XMVECTOR	m_xmvShift;

private:	//player data
	float m_fSpeed{ 0.f };
	INT m_SLOT_ID{ 0 };
public:
	void	SetPawNum(int iPawnNum) { m_iPawnNum = iPawnNum; }
	virtual void RegistToContainer();

public:
	void TransferCollisioinData(int target_slot_id, int skillnum);
	virtual void PhisicsLogic(map<utag, list<CGameObject*>>& mlpObject, float fDeltaTime);
	//demage proc
	virtual bool GetDemaged(int iDemage);
	virtual void GetSkilled(int nSkill);
	virtual int GetSlotID() { return m_SLOT_ID; }
public:
	CDementor(string name, tag t = tag::TAG_DEFAULT, bool bSprit = false, CGameObject* pWeapon = nullptr, INT slot_id = 0);
	virtual ~CDementor();
};

