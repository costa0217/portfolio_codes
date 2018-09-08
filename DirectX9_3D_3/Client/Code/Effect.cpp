#include "StdAfx.h"
#include "Effect.h"
#include "Pipeline.h"
#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"

#include "Export_Function.h"
#include "Include.h"
#include "MathMgr.h"


CEffect::CEffect(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_dwVtxCnt(0)
, m_fDieTime(0)
, m_fAngleZ(0)
{
}

CEffect::~CEffect(void)
{
}

HRESULT CEffect::Initialize(void)
{
	return S_OK;
}

int CEffect::Update(void)
{
	return 0;
}

void CEffect::Render(void)
{

}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEffect* pObj = new CEffect(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

void CEffect::Move(void)
{

}


void CEffect::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		Engine::CMathMgr::MyRotationZ(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, m_fAngleZ);

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

HRESULT CEffect::AddComponent(void)
{
	return S_OK;
}

void CEffect::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}


void CEffect::SetPos(const D3DXVECTOR3& rPos)
{
	m_pInfo->m_vPos = rPos;
}

void CEffect::FlameMove()
{
	m_fFrameCnt += m_fFrameMax * m_pTimeMgr->GetTime() * m_fSpeed;

	if(m_fFrameCnt > m_fFrameMax)
		m_fFrameCnt = 0.f;
}

void CEffect::SetPosF(D3DXVECTOR3 vPos)
{
	m_pInfo->m_vPos = vPos;
}
