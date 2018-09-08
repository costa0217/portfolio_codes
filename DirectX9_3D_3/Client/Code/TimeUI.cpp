#include "stdafx.h"
#include "TimeUI.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"
#include "TerrainCol.h"
#include "CollisionMgr.h"
#include "Stage.h"

CTimeUI::CTimeUI(LPDIRECT3DDEVICE9 pDevice)
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
, m_dwVtxCnt(0)
, m_Min(0)
, m_Sec(0)
{
	ZeroMemory(m_szTime, sizeof(TCHAR) * 128);
}

CTimeUI::~CTimeUI(void)
{
	Release();
}

HRESULT CTimeUI::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = D3DXVECTOR3(-0.71f, 1.f, 0.f);
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	return S_OK;
}

int CTimeUI::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

	Engine::CGameObject::Update();

	TransformMatrix();
	CntTime();

	return 0;
}

void CTimeUI::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pTexture->Render(0);
	m_pBuffer->Render();

	RECT		rc = {83, 43, 500, 500};
	wsprintf(m_szTime, L"%02d : %02d", int(m_Min), int(m_Sec));
	Engine::Get_Management()->GetRenderer()->GetFont()->DrawText(NULL, m_szTime, lstrlen(m_szTime), &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 237, 169, 0));


	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);
}

CTimeUI* CTimeUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTimeUI* pObj = new CTimeUI(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CTimeUI::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.14f, 0.07f, 1.f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TimeUI");
	m_pTexture = static_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CTimeUI::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CTimeUI::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView	= m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);

	const D3DXMATRIX* pmatOrtho	= m_pCameraObserver->GetOrtho();
	NULL_CHECK(pmatOrtho);

	D3DXMATRIX matInv;

	D3DXMatrixIdentity(&matInv);
	D3DXMatrixInverse(&matInv, NULL, pmatView);

	m_pInfo->m_matWorld = m_pInfo->m_matWorld * matInv;


	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatOrtho);
		m_pConvertVertex[i].vPos.x  = int(m_pConvertVertex[i].vPos.x * 100) / 100.f;
		m_pConvertVertex[i].vPos.y  = int(m_pConvertVertex[i].vPos.y * 100) / 100.f;
		m_pConvertVertex[i].vPos.z  = int(m_pConvertVertex[i].vPos.z * 100) / 100.f;
	}
}

void CTimeUI::CntTime(void)
{
	m_Sec += m_pTimeMgr->GetTime();

	if(m_Sec >= 60)
	{
		m_Sec = 0;
		++m_Min;
	}
}

void CTimeUI::SetPos(const D3DXVECTOR3& rPos)
{
	m_pInfo->m_vPos = rPos;
}
