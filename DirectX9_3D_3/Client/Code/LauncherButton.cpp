#include "stdafx.h"
#include "LauncherButton.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"

#include "MouseCol.h"
#include "KeyMgr.h"
#include "ButtonMgr.h"
#include "WeaponStatus.h"
#include "GunButton.h"
#include "GatlingButton.h"
#include "Player.h"

CLauncherButton::CLauncherButton(LPDIRECT3DDEVICE9 pDevice)
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

CLauncherButton::~CLauncherButton(void)
{
	Release();
}

HRESULT CLauncherButton::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos		= D3DXVECTOR3(0.47f, -0.93f, 0.f);
	m_pInfo->m_vScale	= D3DXVECTOR3(0.08f, 0.12f, 0.f);

	m_pCameraObserver	= CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt			= 4;
	m_pVertex			= new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	m_iButton			= 0;
	m_bClick			= false;

	m_pMapObjlist = CButtonMgr::GetInstance()->getlist();

	return S_OK;
}

int CLauncherButton::Update(void)
{
	MouseOver();

	Engine::CGameObject::Update();

	TransformMatrix();


	return 0;
}

void CLauncherButton::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pDevice->SetTexture(0, NULL);

	m_pTexture->Render(m_iButton);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);
}

CLauncherButton* CLauncherButton::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLauncherButton* pObj = new CLauncherButton(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CLauncherButton::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;	

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Launcher");
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

void CLauncherButton::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
	CButtonMgr::GetInstance()->DestroyInstance();
}

void CLauncherButton::TransformMatrix(void)
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
	}
}

void CLauncherButton::MouseOver(void)
{

	POINT ptMouse = CMouseCol::GetMousePos();

	D3DXVECTOR3 vMousePos;
	vMousePos.x = (float)ptMouse.x;
	vMousePos.y = (float)ptMouse.y;
	vMousePos.z = 0.f;


	if(vMousePos.x >= 576 && vMousePos.x <=641 
		&&vMousePos.y >=516 && vMousePos.y <=573)
	{
		m_iButton = 1;
		m_pInfo->m_vPos		= D3DXVECTOR3(0.47f, -1.f, 0.f);

		if(m_pKeyMgr->KeyDown(KEY_LBUTTON))
		{
			m_bClick ^= true;
			m_iButton = 2;
		}
	}

	else
	{
		m_iButton = 0;

		if(m_bClick)
		{
			m_iButton = 2;
			MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"RWeaponStatus");
			MAPOBJLIST::iterator	iter2 = m_pMapObjlist->find(L"ZGunButton");
			MAPOBJLIST::iterator	iter3 = m_pMapObjlist->find(L"ZGatlingButton");
			MAPOBJLIST::iterator	iter4 = m_pMapObjlist->find(L"Player");

			((CWeaponStatus*)iter->second)->ChangeImage(2);

			((CGunButton*)iter2->second)->SetClick(false);
			((CGunButton*)iter2->second)->ChangeImage(0);

			((CGatlingButton*)iter3->second)->SetClick(false);
			((CGatlingButton*)iter3->second)->ChangeImage(0);

			((CPlayer*)iter4->second)->WeaponCreate(L"Buffer_Launcher", WEAPON_LAUNCHER);
		}
		m_pInfo->m_vPos		= D3DXVECTOR3(0.47f, -0.99f, 0.f);
	}



	if(m_bClick)
	{
		MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"RWeaponStatus");
		MAPOBJLIST::iterator	iter2 = m_pMapObjlist->find(L"ZGunButton");
		MAPOBJLIST::iterator	iter3 = m_pMapObjlist->find(L"ZGatlingButton");
		MAPOBJLIST::iterator	iter4 = m_pMapObjlist->find(L"Player");

		((CWeaponStatus*)iter->second)->ChangeImage(2);

		((CGunButton*)iter2->second)->SetClick(false);
		((CGunButton*)iter2->second)->ChangeImage(0);

		((CGatlingButton*)iter3->second)->SetClick(false);
		((CGatlingButton*)iter3->second)->ChangeImage(0);

		((CPlayer*)iter4->second)->WeaponCreate(L"Buffer_Launcher", WEAPON_LAUNCHER);
	}

}

void CLauncherButton::ChangeImage(int image)
{
	m_iButton = image;
}

void CLauncherButton::SetClick(bool click)
{
	m_bClick = click;
}

//bool CHeliStatus::GetBool(void)
//{
//	return m_GotoMain;
//}