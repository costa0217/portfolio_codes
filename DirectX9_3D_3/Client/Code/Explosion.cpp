#include "StdAfx.h"
#include "Explosion.h"

#include "Export_Function.h"
#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "CameraObserver.h"
#include "MathMgr.h"
#include "TimeMgr.h"


CExplosion::CExplosion( LPDIRECT3DDEVICE9 pDevice )
:CEffect(pDevice)
, m_fFrameCnt(0.f)
, m_fFrameMax(0.f)
{

}

CExplosion::~CExplosion( void )
{
	Release();
}

CExplosion* CExplosion::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CExplosion* pObj = new CExplosion(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CExplosion::Initialize( void )
{
	FAILED_CHECK(AddComponent());

	m_fFrameMax = 10.f;

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	m_fSpeed = 30.f;
	//m_vScale = D3DXVECTOR3(1.2f,1.2f,1.2f);

	return S_OK;
}

int CExplosion::Update( void )
{
	m_fFrameCnt += m_fFrameMax * m_pTimeMgr->GetTime() * 0.8f;

	if(m_fFrameCnt > m_fFrameMax)
	{
		m_fFrameCnt = 0.f;
		m_pInfoSubject->UnSubscribe(m_pCameraObserver);
		return 1;
	}
	Engine::CGameObject::Update();

	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = *m_pCameraObserver->GetView();

	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);


	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	TransformMatrix();
	Move();

	

	return 0;
}

void CExplosion::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pTexture->Render((DWORD)m_fFrameCnt);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);
}

void CExplosion::Move( void )
{

}

HRESULT CExplosion::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	float fRand = rand()%5 * 0.1f;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(fRand+20.f,fRand+20.f,fRand+20.f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Explosion");
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

