#include "stdafx.h"
#include "Cloud.h"

#include "Export_Function.h"
#include "Include.h"

#include "Transform.h"
#include "CameraObserver.h"

#include "Stage.h"

CCloud::CCloud(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pManagement(Engine::Get_Management())
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_dwVtxCnt(0)
, m_wstrBufferName(L"")
{

}

CCloud::~CCloud(void)
{
	Release();
}

HRESULT CCloud::Initialize(void)
{
	FAILED_CHECK(AddComponent());


	m_pCameraObserver	= CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt			= m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pVertex);

	return S_OK;
}

int CCloud::Update(void)
{
	Engine::CGameObject::Update();

	TransformMatrix();

	return 0;
}

void CCloud::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pConvertVertex);

	m_pDevice->SetTexture(0, NULL);

	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pVertex);
}

CCloud* CCloud::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCloud* pObj = new CCloud(pDevice);

	return pObj;
}

HRESULT CCloud::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, m_wstrBufferName);
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CCloud::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CCloud::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		if(m_pInfo->m_eState == Engine::STATE_DIE)
			m_pConvertVertex[i].dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

void CCloud::SetPos(const D3DXVECTOR3& rPos)
{
	m_pInfo->m_vPos = rPos;
}

void CCloud::SetBufferName(wchar_t* wstrBufferName)
{
	m_wstrBufferName = wstrBufferName;
}

void CCloud::SetScale(const D3DXVECTOR3& rScale)
{
	m_pInfo->m_vScale = rScale;
}

void CCloud::SetAngleY(float fAngle)
{
	m_pInfo->m_fAngle[Engine::ANGLE_Y] = fAngle;
}

