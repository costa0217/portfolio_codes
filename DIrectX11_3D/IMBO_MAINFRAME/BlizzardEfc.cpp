#include "stdafx.h"
#include "BlizzardEfc.h"

CBlizzardEfc::CBlizzardEfc(string name, tag t)
	:CGameObject(name, t)
{
}

CBlizzardEfc::~CBlizzardEfc()
{
}

void CBlizzardEfc::InitData()
{
	CEffectMgr::GetInstance()->Play_Effect(L"Boss2_sk3_iceSmoke", XMVectorSet(m_xmf3Position.x, 3.f, m_xmf3Position.z, 1.f),
		XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

	m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf3Rotate = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf4Scale = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	m_fAccTime = 0.f;
}

void CBlizzardEfc::DisappearSkill()
{
}

bool CBlizzardEfc::Begin()
{
	CGameObject::Begin();
	return false;
}

void CBlizzardEfc::Initialize()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	// 크기 조정
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(+10.f, +10.f, +10.f, 0.f), XMVectorSet(-10.f, -10.f, -10.f, 0.f));

	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}

void CBlizzardEfc::Animate(float fTimeElapsed)
{
	if (false == m_bActive) return;
	m_fAccTime += fTimeElapsed;

	if (m_fAccTime > 3.f) {
		m_bActive = false;
		InitData();
	}

	CGameObject::Animate(fTimeElapsed);
}

void CBlizzardEfc::RegistToContainer()
{
	CGameObject::RegistToContainer();
}
