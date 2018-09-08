#include "stdafx.h"
#include "BossMeteo.h"


CBossMeteo::CBossMeteo(string name, tag t, bool bStrong)
	:CGameObject(name, t)
{
	m_bStrong = bStrong;
}

CBossMeteo::~CBossMeteo()
{
	if (m_pArrowTrail)
		delete m_pArrowTrail;
}

void CBossMeteo::InitData()
{
	m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf3Rotate = XMFLOAT3(0.f, 0.f, 0.f);
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	m_fAccTime = 0.f;
	m_fTraceTime = 0.f;
}

void CBossMeteo::DisappearSkill()
{
	CEffectMgr::GetInstance()->Play_Effect(L"SparkTest", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.f),
		XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));

	m_bActive = false;
	InitData();
}

void CBossMeteo::SetActive(bool b)
{
	m_bActive = b;
	XMFLOAT4 xmStart, xmEnd;
	XMStoreFloat4(&xmStart, XMVector3TransformCoord(
		XMVectorSet(m_OriBoundingBox.Center.x, m_OriBoundingBox.Extents.y, m_OriBoundingBox.Center.z, 1.f), XMLoadFloat4x4(&m_xmf4x4World)));
	XMStoreFloat4(&xmEnd, XMVector3TransformCoord(
		XMVectorSet(m_OriBoundingBox.Center.x, m_OriBoundingBox.Center.y - m_OriBoundingBox.Extents.y, m_OriBoundingBox.Center.z, 1.f), XMLoadFloat4x4(&m_xmf4x4World)));
	//m_pArrowTrail->SetInitPos(XMLoadFloat4(&xmStart), XMLoadFloat4(&xmEnd));
}

bool CBossMeteo::Begin()
{
	CGameObject::Begin();
	return false;
}

void CBossMeteo::Initialize()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	// 크기 조정
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(+10.f, +10.f, +10.f, 0.f), XMVectorSet(-10.f, -10.f, -10.f, 0.f));

	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}

void CBossMeteo::Animate(float fTimeElapsed)
{
	if (false == m_bActive) return;
	m_fAccTime += fTimeElapsed;
	//if (true == m_bStrong)
	{
		m_fTraceTime += fTimeElapsed;
		if (m_fTraceTime > 0.2f) {
			CEffectMgr::GetInstance()->Play_Effect(L"Wizard_shot_trail", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			m_fTraceTime = 0.f;
		}
	}


	if (m_fAccTime > 3.f) {
		m_bActive = false;
		InitData();
	}
	//if (m_bActive != m_bActive) InitData();


	CGameObject::Animate(fTimeElapsed);


	XMVECTOR xmDir;
	xmDir = XMVector3Normalize(GetLook());
	if(m_bStrong)
		xmDir = XMVector3TransformNormal(xmDir, XMMatrixRotationY(m_fAccTime));

	//float fScaleSize = cosf(m_fAccTime) * 0.02f; 
	//m_xmf4Scale.x += fScaleSize;
	//m_xmf4Scale.y += fScaleSize;
	//m_xmf4Scale.z += fScaleSize;
	XMStoreFloat3(&m_xmf3Position, XMLoadFloat3(&m_xmf3Position) + ((xmDir * 30.f) * fTimeElapsed));
	SetPosition(XMLoadFloat3(&m_xmf3Position));


	//SetRotation(XMMatrixRotationY(m_fAccTime));
	m_bAlive = m_bActive;
}

void CBossMeteo::RegistToContainer()
{
	//m_pArrowTrail->RegistToContainer();
	CGameObject::RegistToContainer();
}
