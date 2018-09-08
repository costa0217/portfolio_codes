#include "stdafx.h"
#include "Freight.h"

#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "Transform.h"
#include "Texture.h"
#include "CameraObserver.h"

#include "TimeMgr.h"
#include "MathMgr.h"
#include "Hook.h"

CFreight::CFreight( LPDIRECT3DDEVICE9 pDevice )
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_pParentInfo(NULL)
, m_bArrive(false)
{

}

CFreight::~CFreight( void )
{
	Release();
}	

HRESULT CFreight::Initialize( void )
{
	FAILED_CHECK(AddComponent());
	m_pInfo->m_vPos = D3DXVECTOR3(0.f,-5.f,0.f);
	m_pInfo->m_vScale = D3DXVECTOR3(5.f,5.f, 5.f);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 8;
	m_pVertex			= new Engine::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCUBE[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

	return S_OK;
}

int CFreight::Update( void )
{
	
	Engine::CGameObject::Update();
	
	if(m_pParentInfo != NULL)
	{
		m_pInfo->m_matWorld = m_pInfo->m_matWorld * m_pParentInfo->m_matWorldNoSca;
	}

	if( (m_pInfo->m_matWorld._41 > 1270.f && m_pInfo->m_matWorld._41 < 1300.f)
		&& m_pInfo->m_matWorld._42 < 10.f&& m_pInfo->m_matWorld._42 > 0.f)
	{
		if(m_bArrive == false)
		{
			Hang = false;
			HangCount += 1;
			m_bArrive = true;
			((CHook*)m_pHook)->SetPos(D3DXVECTOR3(m_pInfo->m_matWorld._41,m_pInfo->m_matWorld._42,m_pInfo->m_matWorld._43));
			((CHook*)m_pHook)->SetParentInfo(NULL);
		}
	}

	TransformMatrix();

	return 0;
}

void CFreight::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex);
	m_pDevice->SetTexture(0, NULL);
	m_pTexture->Render(14);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

}


HRESULT CFreight::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	//((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.3f,0.3f,0.3f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Fragment");
	m_pTexture = static_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

CFreight* CFreight::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CFreight* pObj = new CFreight(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

void CFreight::TransformMatrix( void )
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

void CFreight::Release( void )
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

