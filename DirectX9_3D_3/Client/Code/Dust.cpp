#include "StdAfx.h"
#include "Dust.h"

#include "Export_Function.h"
#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "CameraObserver.h"
#include "MathMgr.h"
#include "TimeMgr.h"




CDust::CDust(LPDIRECT3DDEVICE9 pDevice)
:CEffect(pDevice)
,m_vDir(D3DXVECTOR3(0.f,0.f,1.f))
{

}

CDust::~CDust(void)
{
	Release();
}

HRESULT CDust::Initialize(void)
{

	FAILED_CHECK(AddComponent());

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	m_fSpeed = 30.f;
	m_vScale = D3DXVECTOR3(1.2f,1.2f,1.2f);

	return S_OK;
}

void CDust::DustRotationY()
{
	Engine::CMathMgr::MyRotationY(&m_vDir,&m_vDir,m_fAngleZ);
}

int CDust::Update(void)
{

	Engine::CGameObject::Update();

	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = *m_pCameraObserver->GetView();

	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);


	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	TransformMatrix();
	Move();

	if(m_fDieTime >= 10)
	{
		m_pInfoSubject->UnSubscribe(m_pCameraObserver);
		return 1;
	}

	return 0;
}

void CDust::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pTexture->Render(0);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);
}

void CDust::Move(void)
{
	m_vScale -= D3DXVECTOR3(0.003f,0.003f,0.003f);
	m_pInfo->m_vPos += m_vDir * m_fSpeed * m_pTimeMgr->GetTime();
	m_fDieTime += m_pTimeMgr->GetTime() * 10.f;
	m_pInfo->SetScale(m_vScale);
}

HRESULT CDust::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	float fRand = rand()%20 * 0.1f;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(fRand+0.7f,fRand+0.7f,fRand+0.7f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Dust");
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

CDust* CDust::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CDust* pObj = new CDust(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}