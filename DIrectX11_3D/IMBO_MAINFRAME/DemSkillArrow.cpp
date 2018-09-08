#include "stdafx.h"
#include "DemSkillArrow.h"



CDemSkillArrow::CDemSkillArrow(string name, tag t, bool bStrong)
	:CGameObject(name, t)
{
	//m_pArrowTrail = new CTrail("Trail", tag::TAG_DYNAMIC_OBJECT);
	//->Begin();

	m_pArrowTrail = new CTrail(XMVectorSet(0.3f, 0.3f, 0.6f, 1.f), 0, 0.1f);
	m_pArrowTrail->Initialize();
	m_pArrowTrail->SetTexName(CString("Trail01"));

	m_bStrong = bStrong;
}

CDemSkillArrow::~CDemSkillArrow()
{
	if (m_pArrowTrail)
		delete m_pArrowTrail;
}

void CDemSkillArrow::InitData()
{
	m_xmf3Position = XMFLOAT3(0.f, 0.f, 0.f);
	m_xmf3Rotate = XMFLOAT3(0.f, 0.f, 0.f);
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	m_fAccTime = 0.f;
	m_fTraceTime = 0.f;

}
void CDemSkillArrow::DisappearSkill()
{
	CEffectMgr::GetInstance()->Play_Effect(L"SparkTest", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.f),
		XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));

	m_bActive = false;
	InitData();
}

void CDemSkillArrow::SetActive(bool b)
{
	/*m_bActive = b;
	if (false == m_bActive)
	{
	DisappearSkill();
	}*/
	m_bActive = b;
	XMFLOAT4 xmStart, xmEnd;
	XMStoreFloat4(&xmStart, XMVector3TransformCoord(
		XMVectorSet(m_OriBoundingBox.Center.x, m_OriBoundingBox.Extents.y, m_OriBoundingBox.Center.z, 1.f), XMLoadFloat4x4(&m_xmf4x4World)));
	XMStoreFloat4(&xmEnd, XMVector3TransformCoord(
		XMVectorSet(m_OriBoundingBox.Center.x, m_OriBoundingBox.Center.y - m_OriBoundingBox.Extents.y, m_OriBoundingBox.Center.z, 1.f), XMLoadFloat4x4(&m_xmf4x4World)));
	m_pArrowTrail->SetInitPos(XMLoadFloat4(&xmStart), XMLoadFloat4(&xmEnd));
}

bool CDemSkillArrow::Begin()
{

	CGameObject::Begin();
	return false;
}

void CDemSkillArrow::Initialize()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
	// 크기 조정
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(+10.f, +10.f, +10.f, 0.f), XMVectorSet(-10.f, -10.f, -10.f, 0.f));

	XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
}

void CDemSkillArrow::Animate(float fTimeElapsed)
{
	if (false == m_bActive) return;
	m_fAccTime += fTimeElapsed;
	if (true == m_bStrong)
	{
		m_fTraceTime += fTimeElapsed;
		if (m_fTraceTime > 0.2f) {
			CEffectMgr::GetInstance()->Play_Effect(L"Arrow_Trace", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.f),
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

	if (m_pArrowTrail)
	{
		if (m_bAlive != m_bActive ||
			(m_xmf3Position.x == 0 &&
				m_xmf3Position.y == 0 &&
				m_xmf3Position.z == 0))
		{
			XMFLOAT4 xmStart, xmEnd;
			XMStoreFloat4(&xmStart, XMVector3TransformCoord(
				XMVectorSet(m_OriBoundingBox.Center.x + 0.5f, m_OriBoundingBox.Extents.y, m_OriBoundingBox.Center.z, 1.f), XMLoadFloat4x4(&m_xmf4x4World)));
			XMStoreFloat4(&xmEnd, XMVector3TransformCoord(
				XMVectorSet(m_OriBoundingBox.Center.x - 0.5f, m_OriBoundingBox.Center.y - m_OriBoundingBox.Extents.y, m_OriBoundingBox.Center.z, 1.f), XMLoadFloat4x4(&m_xmf4x4World)));
			m_pArrowTrail->SetInitPos(XMLoadFloat4(&xmStart), XMLoadFloat4(&xmEnd));
		}
		else
		{
			XMFLOAT4 xmStart, xmEnd;
			XMStoreFloat4(&xmStart, XMVector3TransformCoord(
				XMVectorSet(m_OriBoundingBox.Center.x + 0.5f, m_OriBoundingBox.Extents.y, m_OriBoundingBox.Center.z, 1.f), XMLoadFloat4x4(&m_xmf4x4World)));
			XMStoreFloat4(&xmEnd, XMVector3TransformCoord(
				XMVectorSet(m_OriBoundingBox.Center.x - 0.5f, m_OriBoundingBox.Center.y - m_OriBoundingBox.Extents.y, m_OriBoundingBox.Center.z, 1.f), XMLoadFloat4x4(&m_xmf4x4World)));
			m_pArrowTrail->SetStartPos(XMLoadFloat4(&xmStart), XMLoadFloat4(&xmEnd));
			//m_pArrowTrail->SetWorld(XMLoadFloat4x4(&m_xmf4x4World));
			m_pArrowTrail->Update(fTimeElapsed);
		}
	}

	XMStoreFloat3(&m_xmf3Position, XMLoadFloat3(&m_xmf3Position) + ((XMVector3Normalize(GetLook()) * 40.f) * fTimeElapsed));
	SetPosition(XMLoadFloat3(&m_xmf3Position));

	m_bAlive = m_bActive;
}

void CDemSkillArrow::RegistToContainer()
{
	//m_pArrowTrail->RegistToContainer();
	CGameObject::RegistToContainer();
}
