#include "StdAfx.h"
#include "Flare.h"

#include "Export_Function.h"
#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "CameraObserver.h"




CFlare::CFlare(LPDIRECT3DDEVICE9 pDevice)
:CEffect(pDevice)
{

}

CFlare::~CFlare(void)
{
	Release();
}

HRESULT CFlare::Initialize(void)
{

	FAILED_CHECK(AddComponent());

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	m_fSpeed = 30.f;
	m_fFrameCnt = 0.f;
	m_fFrameMax = 4.f;

	return S_OK;
}

int CFlare::Update(void)
{
	Engine::CGameObject::Update();

	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = *m_pCameraObserver->GetView();

	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);


	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	TransformMatrix();

	FlameMove();
	Move();

	if(m_fDieTime >= 30)
	{
		m_pInfoSubject->UnSubscribe(m_pCameraObserver);
		return 1;
	}

	return 0;
}

void CFlare::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pTexture->Render((DWORD)m_fFrameCnt);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);
}

void CFlare::Move(void)
{
	m_pInfo->m_vPos.y += m_pTimeMgr->GetTime() * m_fSpeed;
	m_fAngleZ += D3DXToRadian(45.f) * m_pTimeMgr->GetTime();
	m_fDieTime += m_pTimeMgr->GetTime() * 10.f;
}

HRESULT CFlare::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	float fRand = float(rand()%10);

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(fRand+7.f,fRand+7.f,fRand+7.f));
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Flare");
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

CFlare* CFlare::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFlare* pObj = new CFlare(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}