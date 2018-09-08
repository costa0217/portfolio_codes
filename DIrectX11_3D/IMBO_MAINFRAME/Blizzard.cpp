#include "stdafx.h"
#include "Blizzard.h"




CBlizzard::CBlizzard(string name, tag t)
	:CGameObject(name, t)
{
}

CBlizzard::~CBlizzard()
{
}

void CBlizzard::InitData()
{
	m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf3Rotate = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf4Scale = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	m_fAccTime = 0.f;
}

void CBlizzard::DisappearSkill()
{

}

bool CBlizzard::Begin()
{
	CGameObject::Begin();
	return false;
}

void CBlizzard::Initialize()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	// 크기 조정
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(+10.f, +10.f, +10.f, 0.f), XMVectorSet(-10.f, -10.f, -10.f, 0.f));

	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}

void CBlizzard::Animate(float fTimeElapsed)
{
	if (false == m_bActive) return;
	m_fAccTime += fTimeElapsed;

	if (m_fAccTime > 5.f) {
		m_bActive = false;
		InitData();
	}

	float fScale = 1.f + (m_fAccTime * m_fAccTime) * 2.f;
	m_xmf4Scale = XMFLOAT4(fScale, fScale, fScale, 1.f);
	XMMATRIX matRot = XMMatrixRotationY(m_fAccTime * 1.5f);
	XMMATRIX matPos = XMMatrixTranslation(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z);

	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixScaling(fScale, fScale, fScale) * matRot * matPos);

	CGameObject::Animate(fTimeElapsed);
}

void CBlizzard::RegistToContainer()
{
	CGameObject::RegistToContainer();
}
