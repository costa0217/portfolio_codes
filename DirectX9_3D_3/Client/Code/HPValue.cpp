#include "stdafx.h"
#include "HPValue.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PlayerObserver.h"
#include "Pipeline.h"
#include "TerrainCol.h"
#include "CollisionMgr.h"
#include "Stage.h"

CHPValue::CHPValue(LPDIRECT3DDEVICE9 pDevice)
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
, m_fHpvalue(0)
, m_fDamage(0)
{

}

CHPValue::~CHPValue(void)
{
	Release();
}

HRESULT CHPValue::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fMax		= 100.f; // ÃÖ´ë hp°ª

	m_pInfo->m_vPos		= D3DXVECTOR3(0.0270f, -0.798f, 0.f);
	m_pInfo->m_vScale	= D3DXVECTOR3(0.245f, 0.0362f, 1.f);

	m_pCameraObserver	= CCameraObserver::Create();
	m_pPlayerObserver	= CPlayerObserver::Create();

	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);

	m_dwVtxCnt			= 4;
	m_pVertex			= new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	return S_OK;
}

int CHPValue::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);

	Engine::CGameObject::Update();

	TransformMatrix();
	GetPlayerInfo();


	if(m_fHpvalue > 2)
	{
		m_fHpvalue = 2;
	}

	return 0;
}

void CHPValue::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pTexture->Render(1);
	m_pBuffer->Render();
}

CHPValue* CHPValue::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHPValue* pObj = new CHPValue(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CHPValue::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	//((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.418f, 0.054f,1.f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_HPBar");
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

void CHPValue::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	m_pInfoSubject->UnSubscribe(m_pPlayerObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
	Engine::Safe_Delete(m_pPlayerObserver);

}

void CHPValue::TransformMatrix(void)
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

		if( i==1 || i==2 )
		{
			m_pConvertVertex[i].vPos.x -= m_fHpvalue;
			m_pConvertVertex[i].vTexUV.x -= m_fHpvalue/2.f;
		}

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatOrtho);
		//m_pConvertVertex[i].vPos.x  = int(m_pConvertVertex[i].vPos.x * 100) / 100.f;
		//m_pConvertVertex[i].vPos.y  = int(m_pConvertVertex[i].vPos.y * 100) / 100.f;
		//m_pConvertVertex[i].vPos.z  = int(m_pConvertVertex[i].vPos.z * 100) / 100.f;
	}
}

void CHPValue::GetPlayerInfo(void)
{
	pPlayerHP = m_pPlayerObserver->GetHp();


	//m_fDamage = m_fMax - pPlayerHP;

	m_fHpvalue = 2 - 2 * pPlayerHP/100;
	// -1  1
	//  2  0
}

void CHPValue::SetPos(const D3DXVECTOR3& rPos)
{
	m_pInfo->m_vPos = rPos;
}
