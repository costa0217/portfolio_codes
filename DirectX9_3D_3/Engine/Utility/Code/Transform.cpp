#include "Transform.h"
#include "Pipeline.h"

Engine::CTransform::CTransform(const D3DXVECTOR3& vLook)
: m_vPos(0.f, 0.f, 0.f)
, m_vDir(vLook)
, m_fHP(0.f)
{
	m_vScale = D3DXVECTOR3(1.f, 1.f, 1.f);
	ZeroMemory(m_fAngle, sizeof(float) * ANGLE_END);
	ZeroMemory(m_fAngleNo, sizeof(float) * ANGLE_END);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matWorldAng);
	D3DXMatrixIdentity(&m_matWorldNoSca);
}

Engine::CTransform::~CTransform(void)
{
	
}

void Engine::CTransform::Update(void)
{
	CPipeline::MakeWorldMatrix(&m_matWorld, &m_vScale
		, m_fAngle, &m_vPos);
	
	//주석풀어져있는것.
	CPipeline::MakeWorldMatrixAng(&m_matWorldAng, &m_vScale
		, m_fAngleNo, &m_vPos);

	CPipeline::MakeWorldMatrix(&m_matWorldNoSca, &D3DXVECTOR3(1.f, 1.f, 1.f)
		, m_fAngle, &m_vPos);

}

Engine::CTransform* Engine::CTransform::Create(const D3DXVECTOR3& vLook)
{
	return new CTransform(vLook);
}

