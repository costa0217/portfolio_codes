#include "stdafx.h"
#include "StartButton.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"

#include "MouseCol.h"
#include "KeyMgr.h"

CStartButton::CStartButton(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pKeyMgr(Engine::Get_KeyMgr())
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_dwVtxCnt(0)
{

}

CStartButton::~CStartButton(void)
{
	Release();
}

HRESULT CStartButton::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos		= D3DXVECTOR3(0.f, -0.52f, 0.f);
	m_pInfo->m_vScale	= D3DXVECTOR3(0.08f, 0.15f, 0.f);

	m_pCameraObserver	= CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt			= 4;
	m_pVertex			= new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	m_iButton			= 0;
	m_ClickStart		= false;

	return S_OK;
}

int CStartButton::Update(void)
{
	MouseOver();

	Engine::CGameObject::Update();

	TransformMatrix();


	return 0;
}

void CStartButton::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pDevice->SetTexture(0, NULL);

	m_pTexture->Render(m_iButton);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);
}

CStartButton* CStartButton::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStartButton* pObj = new CStartButton(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CStartButton::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;	

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_StartButton");
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

void CStartButton::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CStartButton::TransformMatrix(void)
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
		m_pConvertVertex[i].vPos.x  = int(m_pConvertVertex[i].vPos.x * 1000) / 1000.f;
		m_pConvertVertex[i].vPos.y  = int(m_pConvertVertex[i].vPos.y * 1000) / 1000.f;
		m_pConvertVertex[i].vPos.z  = int(m_pConvertVertex[i].vPos.z * 1000) / 1000.f;
	}
}

void CStartButton::MouseOver(void)
{
	POINT ptMouse = CMouseCol::GetMousePos();

	D3DXVECTOR3 vMousePos;
	vMousePos.x = (float)ptMouse.x;
	vMousePos.y = (float)ptMouse.y;
	vMousePos.z = 0.f;


	if(vMousePos.x >= 370 && vMousePos.x <= 430 
		&&vMousePos.y >=360 && vMousePos.y <=423)
	{
		m_iButton = 1;
		m_pInfo->m_vPos		= D3DXVECTOR3(0.f, -0.385f, 0.f);

		if(m_pKeyMgr->KeyPressed(KEY_LBUTTON, 2))
		{
			m_ClickStart	= true;
		}
	}
	else
	{
		m_iButton = 0;
		m_pInfo->m_vPos		= D3DXVECTOR3(0.f, -0.38f, 0.f);
	}
}

bool CStartButton::GetBool(void)
{
	return m_ClickStart;
}

