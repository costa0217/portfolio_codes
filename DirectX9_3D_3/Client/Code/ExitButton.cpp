#include "stdafx.h"
#include "ExitButton.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"
#include "MouseCol.h"

CExitButton::CExitButton(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pKeyMgr(Engine::Get_KeyMgr())
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_dwVtxCnt(0)
{

}

CExitButton::~CExitButton(void)
{
	Release();
}

HRESULT CExitButton::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = D3DXVECTOR3(-0.789f, -0.8f, -0.5f);
	m_pInfo->m_vScale = D3DXVECTOR3(0.0765f, 0.213f, 0.f);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pVertex);

	m_iButton = 0;

	return S_OK;
}

int CExitButton::Update(void)
{
	MouseOver();
	Engine::CGameObject::Update();

	TransformMatrix();
	

	return 0;
}

void CExitButton::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pConvertVertex);

	m_pTexture->Render(m_iButton);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pVertex);
}

CExitButton* CExitButton::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CExitButton* pObj = new CExitButton(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CExitButton::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;	

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	//((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.0765f, 0.213f, 0.f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Exit");
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

void CExitButton::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CExitButton::TransformMatrix(void)
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

void CExitButton::MouseOver(void)
{

	Engine::Get_GraphicDev()->SoundPlay(SOUND_BUTTON); 

	POINT ptMouse = CMouseCol::GetMousePos();

	D3DXVECTOR3 vMousePos;
	vMousePos.x = (float)ptMouse.x;
	vMousePos.y = (float)ptMouse.y;
	vMousePos.z = 0.f;

	/*const D3DXMATRIX*	pmatProj = m_pCameraObserver->GetOrtho();

	D3DXVECTOR3 vMousePos;
	vMousePos.x = ((float(ptMouse.x) / (WINCX >> 1)) - 1.f) / pmatProj->_11;
	vMousePos.y = ((float(-ptMouse.y) / (WINCY >> 1)) + 1.f) / pmatProj->_22;
	vMousePos.z = 1.f;

	D3DXMatrixIdentity(&m_pInfo->m_matWorld);

	D3DXMATRIX		matView = *m_pCameraObserver->GetView();

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matView);

	D3DXMATRIX		matWorldInv;

	D3DXMatrixInverse(&matWorldInv, NULL, &m_pInfo->m_matWorld);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matWorldInv);
	
	D3DXVECTOR3 vDistance =  vMousePos - m_pInfo->m_vPos;
	D3DXVec3Normalize(&vDistance, &vDistance);

	float fDistance = D3DXVec3Length(&vDistance);

	if(fDistance < 5.f)
	{
		m_iButton = 1;
	}*/

	if(vMousePos.x >= 20 && vMousePos.x <=80 
		&&vMousePos.y >=500 && vMousePos.y <=580)
	{
		m_iButton = 1;
		m_pInfo->m_vPos = D3DXVECTOR3(-0.789f, -0.815f, -0.5f);
		
		if(m_pKeyMgr->KeyPressed(KEY_LBUTTON, 1))
		{
			PostQuitMessage(0);
		}
	}
	else
	{
		m_iButton = 0;
		m_pInfo->m_vPos = D3DXVECTOR3(-0.789f, -0.8f, -0.5f);
	}
}