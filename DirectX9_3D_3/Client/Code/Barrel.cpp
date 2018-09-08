#include "stdafx.h"
#include "Barrel.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PlayerObserver.h"
#include "Pipeline.h"
#include "CollisionMgr.h"
#include "MathMgr.h"

CBarrel::CBarrel(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pManagement(Engine::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_pTerrainVertex(NULL)
, m_fAngleX(0.f)
, m_fAngleZ(0.f)
, m_fAngleY(0.f)
, m_fDestAngleZ(0.f)
, m_fDestAngleX(0.f)
, m_fDestAngleY(0.f)
, m_bDie(false)
, m_pTarget(NULL)
{

}

CBarrel::~CBarrel(void)
{
	
}

HRESULT CBarrel::Initialize(void)
{
	return S_OK;
}

int CBarrel::Update(void)
{
	return 0;
}

void CBarrel::Render(void)
{

}

CBarrel* CBarrel::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBarrel* pObj = new CBarrel(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CBarrel::AddComponent(void)
{
	return S_OK;
}

void CBarrel::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		Engine::CMathMgr::MyRotationZ(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, m_fAngleZ);
		Engine::CMathMgr::MyRotationX(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, m_fAngleX);
		Engine::CMathMgr::MyRotationY(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, m_fAngleY);

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}	

void CBarrel::Move(void)
{

}

void CBarrel::Release()
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	m_pInfoSubject->UnSubscribe(m_pPlayerObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
	Engine::Safe_Delete(m_pPlayerObserver);
}

void CBarrel::SetPos( D3DXVECTOR3 vPos )
{
	m_pInfo->m_vPos = vPos;
}

