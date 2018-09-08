#include "stdafx.h"
#include "LogoBack.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_fFrameCnt(0.f)
, m_fFrameMax(0.f)
{

}

CLogoBack::~CLogoBack(void)
{
	Release();
}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fFrameMax = 21;

	m_pInfo->m_vPos		= D3DXVECTOR3(0.f, 0.f, 0.f);

	m_pCameraObserver	= CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt			= 4;
	m_pVertex			= new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pVertex);

	return S_OK;
}

int CLogoBack::Update(void)
{
	m_fFrameCnt += m_fFrameMax * m_pTimeMgr->GetTime() * 0.8f;

	if(m_fFrameCnt > m_fFrameMax)
		m_fFrameCnt = 0.f;

	Engine::CGameObject::Update();
	TransformMatrix();

	return 0;
}

void CLogoBack::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pConvertVertex);

	m_pTexture->Render((DWORD)m_fFrameCnt);

	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pVertex);
}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogoBack* pObj = new CLogoBack(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CLogoBack::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;	

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.907f, 1.3f, 1.f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_LogoBack");
	m_pTexture = static_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CLogoBack::Release(void)
{

	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CLogoBack::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView	= m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);

	const D3DXMATRIX* pmatOrtho	= m_pCameraObserver->GetOrtho();
	NULL_CHECK(pmatOrtho);


	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatOrtho);
	}
}