#pragma once

#include "GameObject.h"


struct EffectInfo {
	int			m_iTexOpt;
	float		m_fFrameSpd;
	int			m_iU;
	int			m_iV;
	float		m_fFrameCnt;
	int			m_iFrameHeight;
	float		noUse1;
	float		noUse2;
	XMFLOAT4	m_xmColor;
	XMFLOAT4X4	m_matWorld;
};
class CEffectRenderCont;
class CMyEffect 
{
public:
	CMyEffect();
	~CMyEffect();
public:
	TCHAR		m_szTexture[64];		//텍스처 이름
	float		m_fEffectEndTime{ 0.f };
	float		m_fAccTime{ 0.f };
	CMyEffect*	m_pParent{ nullptr };
	CMyEffect*	m_pOriEffect{ nullptr };

	EffectInfo	m_tEffectInfo;
	 CBuffer*	m_pCBuffer;

	CEffectRenderCont*			m_pEffectRenderCont;

private:
	list<POINTLERP*>		m_PointList;
	bool		m_bPlayEffect{ true };
	float		m_fAccPlayTime{ 0.f };
	int			m_iAniCnt{ -1 };
	
public:
	bool		m_bRender{ true };
	bool		m_bEffectDie{ false };
	bool		m_bBillboard{ false };
	bool		m_bDistortion{ false };
	bool		m_bRepeat{ false };

public:
	bool			m_bAlive{ false };
	CGameObject*	m_pmParent{ nullptr };

public:
	list<POINTLERP*>*	GetVectorLinear() { return &m_PointList; }
	void		SetPlayStartEffect(bool bStart) { m_bPlayEffect = bStart; }
	void		SetTexName(CString name);
	void		SortingPoints();
	void		SettingPoint(POINTLERP* pIter);
	void		StopEffectAniamtion();
	void		Copy_Point(CMyEffect* pCopy);
	void		Delete_PointAll();
	void		Modify_Point(XMVECTOR xmPos, XMVECTOR xmRot, XMVECTOR xmScl);
	void		Modify_Point(CGameObject * pParent);
	void		InitData();
private:
	XMFLOAT4	m_f4Color;
	XMFLOAT4	m_f4Scale{ 1.f, 1.f, 1.f, 1.f };
	XMFLOAT3	m_f3Angle{ 0.f, 0.f, 0.f };
	XMFLOAT4	m_f4Quaternion;
	XMFLOAT3	m_f3Pos{ 0.f, 0.f, 0.f };
public:
	void	SetPos(float x, float y, float z) { m_f3Pos.x = x;	m_f3Pos.y = y;	m_f3Pos.z = z;};
	void	SetAngle(float x, float y, float z) {m_f3Angle.x = x;	m_f3Angle.y = y;	m_f3Angle.z = z;}
	void	SetScale(float x, float y, float z) { m_f4Scale.x = x;	m_f4Scale.y = y;	m_f4Scale.z = z;}
	void	SetColor(float x, float y, float z, float w) { m_f4Color.x = x;	m_f4Color.y = y;	m_f4Color.z = z;  m_f4Color.w = w; }
	XMFLOAT3	GetPos()	{ return m_f3Pos; }
	XMFLOAT3	GetAngle()	{ return m_f3Angle; }
	XMFLOAT4	GetScale()	{ return m_f4Scale; }
	XMFLOAT4	GetColor() { return m_f4Color; }

public:
	HRESULT Initialize();
	int		Update(float fTimeElapsed);
	void	Render(); 
	void	Release();

private:
	void	SetParameter();
	void	MakeQuaternion();
	void	PlayEffectAnimation(float fDeltaTime);
	void	UpdateFrameAnimation(float fDeltaTime);
	void	AdjustBillboard();
};

