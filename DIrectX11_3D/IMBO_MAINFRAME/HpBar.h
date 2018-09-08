#pragma once
#include "UIObject.h"
class CHpBar :
	public CUIObject
{
public:
	CHpBar();
	~CHpBar();


public:
	static CHpBar* Create(XMVECTOR xyPos, XMVECTOR xySize);
	void	SetInfo(XMVECTOR xyPos, XMVECTOR xySize);
	void SetPosition(XMFLOAT2 xyPos);
	void SetSize(XMFLOAT2 xySize);
	float GetCurHPRate() { return m_fCurHPRate; }
	void SetCurHPRate(float fRate) { m_fCurHPRate = fRate; }
	float GetCurHPLength() { return m_fCurHPLength; }
	void SetCurHPLength(float fLength) { m_fCurHPLength = fLength; }
	void SetHPDownSpeed(float fSpeed) { m_fHPDownSpeed = fSpeed; }
	float GetHPDownSpeed() { return m_fHPDownSpeed; }
private:
	 CTexture*	m_pBarTexture;

private:
	int		m_nID{ 0 };
	float m_fCurHPRate{ 1.f };
	float m_fMaxHPLength{ 1.f };
	float m_fCurHPLength{ 0.f };
	float m_fHPDownSpeed{ 1.f };

	float m_fInitPosX{ 0.f };
	float m_fInitSizeX{ 0.f };
public:
	void	SetID(int nID) { m_nID = nID; }
	int		GetID() { return m_nID; }

public:
	virtual HRESULT Initialize();
	virtual int		Update(float fTimeElapsed);
	virtual void	Render();
	virtual void	Release();

private:
	void	SetParameter();
};


