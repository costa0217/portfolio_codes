#include "stdafx.h"
#include "BossExplosion.h"


CBossExplosion::CBossExplosion(string name, tag t)
	:CGameObject(name, t)
{
}

CBossExplosion::~CBossExplosion()
{
}

void CBossExplosion::InitData()
{
	CEffectMgr::GetInstance()->Play_Effect(L"boss2_sk2_expefc", XMVectorSet(m_xmf3Position.x, 3.f, m_xmf3Position.z, 1.f),
		XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

	m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf3Rotate = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf4Scale = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	m_fAccTime = 0.f;

}

void CBossExplosion::DisappearSkill()
{
}

bool CBossExplosion::Begin()
{
	CGameObject::Begin();
	return false;
}

void CBossExplosion::Initialize()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	// 크기 조정
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(+10.f, +10.f, +10.f, 0.f), XMVectorSet(-10.f, -10.f, -10.f, 0.f));

	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}

void CBossExplosion::Animate(float fTimeElapsed)
{
	if (false == m_bActive) return;
	m_fAccTime += fTimeElapsed;
	//if (true == m_bStrong)
	//{
	//	//m_fTraceTime += fTimeElapsed;
	//	if (m_fTraceTime > 0.2f) {
	//		CEffectMgr::GetInstance()->Play_Effect(L"Wizard_shot_trail", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.f),
	//			XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
	//		m_fTraceTime = 0.f;
	//	}
	//}


	if (m_fAccTime > 9.f || m_xmf3Position.y < 0.f) {



		m_bActive = false;
		InitData();
	}
	//if (m_bActive != m_bActive) InitData();


	CGameObject::Animate(fTimeElapsed);


	XMVECTOR xmDir;
	xmDir = XMVector3Normalize(XMVectorSet(0.f, -1.f, 0.f, 1.f));

	XMStoreFloat3(&m_xmf3Position, XMLoadFloat3(&m_xmf3Position) + (xmDir * fTimeElapsed * 40.f));
	SetPosition(XMLoadFloat3(&m_xmf3Position));


	//SetRotation(XMMatrixRotationY(m_fAccTime));
	m_bAlive = m_bActive;
}

void CBossExplosion::RegistToContainer()
{
	CGameObject::RegistToContainer();
}
