#include "StdAfx.h"
#include "Smoke.h"

#include "Export_Function.h"
#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "CameraObserver.h"



CSmoke::CSmoke(LPDIRECT3DDEVICE9 pDevice)
:CEffect(pDevice)
{

}

CSmoke::~CSmoke(void)
{
	Release();
}

HRESULT CSmoke::Initialize(void)
{

	FAILED_CHECK(AddComponent());

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	m_fSpeed = 1.f;
	m_fFrameCnt = 0.f;
	m_fFrameMax = 6.f;
	m_fAngleX = rand()%50*0.1f;

	return S_OK;
}

int CSmoke::Update(void)
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

	if(m_fDieTime >= 10)
	{	
		m_pInfoSubject->UnSubscribe(m_pCameraObserver);
		return 1;
	}

	return 0;
}

void CSmoke::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pTexture->Render((DWORD)m_fFrameCnt);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);
}

void CSmoke::Move(void)
{
	m_pInfo->m_vPos.y += m_pTimeMgr->GetTime() * 10.f;
	m_fAngleZ += D3DXToRadian(45.f) * m_pTimeMgr->GetTime();
	m_fDieTime += m_pTimeMgr->GetTime() * 10.f;
	/*m_pInfo->m_vPos.x  += 0.2f*sin(m_fAngleX);
	m_fAngleX += 0.1f;

	m_fAngleZ += D3DXToRadian(90.f) * m_pTimeMgr->GetTime();
	m_fDieTime += m_pTimeMgr->GetTime() * 10.f;*/
}

HRESULT CSmoke::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	float fRand = float(rand()%3);

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(fRand+0.5f,fRand+0.5f,fRand+0.5f));
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Smoke");
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

CSmoke* CSmoke::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSmoke* pObj = new CSmoke(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}