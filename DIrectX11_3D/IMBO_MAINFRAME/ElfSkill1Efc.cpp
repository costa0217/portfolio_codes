#include "stdafx.h"
#include "ElfSkill1Efc.h"


CElfSkill1Efc::CElfSkill1Efc(string name, tag t)
	:CGameObject(name, t)
{
}

CElfSkill1Efc::~CElfSkill1Efc()
{
}

void CElfSkill1Efc::InitData()
{
	m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf3Rotate = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf4Scale = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	m_fAccTime = 0.f;
}

void CElfSkill1Efc::DisappearSkill()
{

}

bool CElfSkill1Efc::Begin()
{
	CGameObject::Begin();
	return false;
}

void CElfSkill1Efc::Initialize()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	// 크기 조정
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(+10.f, +10.f, +10.f, 0.f), XMVectorSet(-10.f, -10.f, -10.f, 0.f));

	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}

void CElfSkill1Efc::Animate(float fTimeElapsed)
{
	if (false == m_bActive) return;
	m_fAccTime += fTimeElapsed;

	if (m_fAccTime > 0.5f) {
		m_bActive = false;
		InitData();
	}

	float fScale = 1.f + (m_fAccTime + m_fAccTime) * 3.f;
	m_xmf4Scale = XMFLOAT4(fScale, fScale, fScale, 1.f);
	XMMATRIX matRot = XMMatrixRotationY(m_fAccTime * 1.5f);
	XMMATRIX matPos = XMMatrixTranslation(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z);

	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixScaling(fScale, fScale, fScale) * matRot * matPos);

	CGameObject::Animate(fTimeElapsed);

}

void CElfSkill1Efc::RegistToContainer()
{
	CGameObject::RegistToContainer();
}
