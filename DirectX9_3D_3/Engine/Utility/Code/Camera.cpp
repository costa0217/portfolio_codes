#include "Camera.h"
#include "Pipeline.h"

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_vEye(0.f, 0.f, 0.f)
, m_vAt(0.f, 0.f, 0.f)
, m_vUp(0.f, 1.f, 0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matOrtho);
}

Engine::CCamera::~CCamera(void)
{

}

void Engine::CCamera::SetViewSpaceMatrix(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp)
{
	CPipeline::MakeViewSpaceMatrix(&m_matView, pEye, pAt, pUp);
	//m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);	
}

void Engine::CCamera::SetProjectionMatrix(const float& fFovY, const float& fAspece , const float& fNear, const float& fFar)
{
	CPipeline::MakeProjectionMatrix(&m_matProj, fFovY, fAspece, fNear, fFar);
	//m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void Engine::CCamera::SetOrthogonalMatrix(const float& fFovY, const float& fAspece , const float& fNear, const float& fFar)
{
	CPipeline::MakeOrthogonalMatrix(&m_matOrtho, fFovY, fAspece, fNear, fFar);
}